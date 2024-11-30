#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <jansson.h>

#define MAX_URL_LENGTH 512

typedef struct
{
	char	*memory;
	size_t	size;
}	MemoryBlock;

typedef struct
{
    char	*key;
    char	*value;
}	KeyValue;

static size_t	WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	MemoryBlock *mem = (MemoryBlock *)userp;

	char *ptr = realloc(mem->memory, mem->size + realsize + 1);
	if (ptr == NULL)
	{
		fprintf(stderr, "Erro: Memória insuficiente para resposta HTTP\n");
		return 0;
	}

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = '\0';

	return (realsize);
}

char	FIREBASE_URL[MAX_URL_LENGTH];

void	set_db(const char *db_url)
{
	strncpy(FIREBASE_URL, db_url, MAX_URL_LENGTH);
	printf("URL do Firebase configurada para: %s\n", FIREBASE_URL);
}

char	*http_request(const char *endpoint, const char *method, const char *data) {
	CURL *curl;
	CURLcode res;
	MemoryBlock response = {NULL, 0};

	char url[MAX_URL_LENGTH];
	snprintf(url, sizeof(url), "%s/%s.json", FIREBASE_URL, endpoint);

	curl = curl_easy_init();
	if (curl)
	{
		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, "Content-Type: application/json");

		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		if (data)
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
			fprintf(stderr, "Erro na requisição %s: %s\n", method, curl_easy_strerror(res));
		curl_easy_cleanup(curl);
		curl_slist_free_all(headers);
	}
	return (response.memory);
}

void	set_db_value(const char *path, const char *json_data)
{
	http_request(path, "PUT", json_data);
}

char	*push_data(const char *path, const char *json_data)
{
	char *response = http_request(path, "POST", json_data);
    return (response);
}

void	update_data(const char *path, const char *json_data)
{
	http_request(path, "PATCH", json_data);
}

void	delete_data(const char *path)
{
	http_request(path, "DELETE", NULL);
}

char	*get_data(const char *path) {
	return http_request(path, "GET", NULL);
}

char    *json_to_string(const char *json_str)
{
    json_error_t error;
    json_t *root = json_loads(json_str, 0, &error);

    if (!root) {
        fprintf(stderr, "Erro ao analisar JSON: %s\n", error.text);
        return (NULL);
    }

    char *json_str_out = json_dumps(root, JSON_ENCODE_ANY);
    json_decref(root);
    return (json_str_out);
}

KeyValue    *json_to_keyvalue(const char *json_str, size_t *count)
{
    json_error_t error;
    json_t *root = json_loads(json_str, 0, &error);

    if (!root || !json_is_object(root)) {
        fprintf(stderr, "Erro ao analisar JSON ou JSON não é um objeto: %s\n", error.text);
        *count = 0;
        return (NULL);
    }

    size_t size = json_object_size(root);
    KeyValue *keyvalues = malloc(size * sizeof(KeyValue));
    if (!keyvalues) {
        perror("Erro ao alocar memória");
        json_decref(root);
        *count = 0;
        return (NULL);
    }

    const char *key;
    json_t *value;
    size_t i = 0;

    json_object_foreach(root, key, value)
    {
        keyvalues[i].key = strdup(key);
        if (!keyvalues[i].key) {
            perror("Erro ao copiar chave");
            break ;
        }
        const char *value_str = json_is_string(value) ? json_string_value(value) : json_dumps(value, JSON_ENCODE_ANY);
        keyvalues[i].value = strdup(value_str);
        if (!keyvalues[i].value)
        {
            perror("Erro ao copiar valor");
            break ;
        }
        i++;
    }
    *count = i;
    json_decref(root);
    return (keyvalues);
}

char    *post_keyvalue(const KeyValue *keyvalues, size_t count)
{
    json_t *root = json_object();
    char *response = NULL;
    for (size_t i = 0; i < count; i++)
    {
        json_object_set_new(root, keyvalues[i].key, json_string(keyvalues[i].value));
    }

    char *json_data = json_dumps(root, JSON_ENCODE_ANY);
    if (json_data)
    {
        push_data("db", json_data);
        free(json_data);
    }
    json_decref(root);
    return (response);
}

KeyValue    *get_keyvalue(const char *path, size_t *count)
{
    char *response = get_data(path);
    if (!response)
    {
        *count = 0;
        return (NULL);
    }

    KeyValue *keyvalues = json_to_keyvalue(response, count);
    free(response);
    return (keyvalues);
}

KeyValue    *get_keyvalue_id(const char *path, const char *id, size_t *count)
{
    char *response = get_data(path);
    if (!response)
    {
        *count = 0;
        return (NULL);
    }

    json_error_t error;
    json_t *root = json_loads(response, 0, &error);
    free(response);

    if (!root || !json_is_object(root))
    {
        fprintf(stderr, "Erro ao analisar JSON ou JSON não é um objeto: %s\n", error.text);
        *count = 0;
        return (NULL);
    }

    json_t *value = json_object_get(root, id);
    if (!value)
    {
        fprintf(stderr, "ID não encontrado: %s\n", id);
        *count = 0;
        return (NULL);
    }

    KeyValue *keyvalues = json_to_keyvalue(json_dumps(value, JSON_ENCODE_ANY), count);
    json_decref(root);
    return (keyvalues);
}


int	main(void)
{
	set_db("https://sinotasf-default-rtdb.firebaseio.com");

	set_db_value("db", "{\"pessoas\": \"altas\"}");

	char *id =  push_data("db", "{\"nome\": \"João\", \"idade\": 30}");

	update_data("db", "{\"animais\": \"grandes\"}");
    post_keyvalue((KeyValue[]){{"cor", "vermelho"}, {"tamanho", "grande"}}, 2);
    
    char *response = get_data("db");
	printf("Dados obtidos: %s\n", response);

    printf("Convertendo JSON para string:\n");
    char *json_str = json_to_string(response);
    if (json_str == NULL)
    {
        printf("Erro ao converter JSON para string.\n");
        return 1;
    }
    printf("JSON convertido: %s\n", json_str);
    free(json_str);

	printf("Convertendo JSON para matriz:\n");
	size_t	rows;
    KeyValue *keyvalues = json_to_keyvalue(response, &rows);
    if (keyvalues == NULL)
    {
        printf("Erro ao converter JSON para matriz.\n");
        return 1;
    }
    printf("Dados em formato de chave-valor:\n");
    for (size_t i = 0; i < rows; i++)
        printf("Chave: %s, Valor: %s\n", keyvalues[i].key, keyvalues[i].value);

    keyvalues = get_keyvalue_id("db", id, &rows);
    if (keyvalues == NULL)
    {
        printf("Erro ao obter chave-valor por ID.\n");
        return 1;
    }
    printf("Dados em formato de chave-valor por ID:\n");
    for (size_t i = 0; i < rows; i++)
        printf("Chave: %s, Valor: %s\n", keyvalues[i].key, keyvalues[i].value);

	//delete_data("db/animais");
	free(response);
	return 0;
}

