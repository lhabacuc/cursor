import socket
import threading

clients = []

def handle_client(client_socket, addr):
    clients.append(client_socket)
    print(f"Ligação estabelecida com {addr}")
    while True:
        try:
            msg = client_socket.recv(1024).decode('utf-8')
            if not msg:
                break
            if msg == "logout":
                print(f"{addr} fez logout\n")
                break
            print(f"Recebida de {addr}: " + msg + "\n")
            broadcast(msg, client_socket)
        except ConnectionResetError:
            print(f"{addr} desconectou-se abruptamente.")
            break
    clients.remove(client_socket)
    client_socket.close()

def broadcast(msg, sender_socket):
    for client in clients:
        if client != sender_socket:
            try:
                client.send(msg.encode('utf-8'))
            except Exception as e:
                print("Erro ao enviar mensagem:", e)
                client.close()
                clients.remove(client)

def start_server():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    hostname = socket.gethostname()
    local_ip = socket.gethostbyname(hostname)
    server.bind((local_ip, 8000))

    server.listen()
    print(f'Servidor à espera de ligações em {local_ip}:8000...')

    while True:
        client_socket, addr = server.accept()
        client_handler = threading.Thread(target=handle_client, args=(client_socket, addr))
        client_handler.start()

start_server()
