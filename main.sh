#!/bin/bash

# URL base do Firebase
FIREBASE_URL="https://sinotasf-default-rtdb.firebaseio.com"

# Define a URL base do banco de dados
set_db() {
    FIREBASE_URL="$1"
    echo "URL do Firebase configurada para: $FIREBASE_URL"
}

# Envia dados para o Firebase (método PUT)
set_data() {
    local path="$1"
    local json_data="$2"
    curl -s -X PUT -H "Content-Type: application/json" -d "$json_data" "${FIREBASE_URL}/${path}.json"
    echo "Dados definidos em $path."
}

# Adiciona dados com chave única (método POST)
push_data() {
    local path="$1"
    local json_data="$2"
    response=$(curl -s -X POST -H "Content-Type: application/json" -d "$json_data" "${FIREBASE_URL}/${path}.json")
    echo "Dados adicionados com chave única: $response"
}

# Atualiza campos específicos (método PATCH)
update_data() {
    local path="$1"
    local json_data="$2"
    curl -s -X PATCH -H "Content-Type: application/json" -d "$json_data" "${FIREBASE_URL}/${path}.json"
    echo "Dados atualizados em $path."
}

# Remove dados de um caminho específico (método DELETE)
delete_data() {
    local path="$1"
    curl -s -X DELETE "${FIREBASE_URL}/${path}.json"
    echo "Dados removidos de $path."
}

# Obtém dados de um caminho específico (método GET)
get_data() {
    local path="$1"
    curl -s -X GET "${FIREBASE_URL}/${path}.json"
}

# Converte um JSON simples para uma tabela legível
json_to_table() {
    local json="$1"
    echo "$json" | jq -r 'to_entries[] | "\(.key): \(.value)"'
}

main() {
    set_db "https://sinotasf-default-rtdb.firebaseio.com"

    set_data "db" '{"pessoas": "baixas"}'

    push_data "db" '{"nome": "mauro", "idade": 10}'

    update_data "db" '{"animais": "grandes"}'

    response=$(get_data "db")
    echo "Dados obtidos:"
    echo "$response"

    echo "Convertendo JSON para tabela:"
    json_to_table "$response"

    #delete_data "db/animais"

    updated_response=$(get_data "db")
    echo "Dados após remoção:"
    echo "$updated_response"
}

main


exit 0

main() {
    #set_db "https://sinotasf-default-rtdb.firebaseio.com"

    #set_data "db" '{"pessoas": "baixas"}'

    #push_data "db" '{"nome": "mauro", "idade": 10}'

   # update_data "db" '{"animais": "grandes"}'

    response=$(get_data "db/-OCOqCEIULe8tDdNXHEQ")
    echo "Dados obtidos:"
    echo "$response"

}

