#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

std::vector<int> clients;
std::mutex clients_mutex;

void broadcast(const std::string& message, int sender_socket) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (int client : clients) {
        if (client != sender_socket) {
            send(client, message.c_str(), message.size(), 0);
        }
    }
}

void handle_client(int client_socket, sockaddr_in client_addr) {
    char buffer[1024];
    std::cout << "Клиент " << inet_ntoa(client_addr.sin_addr)
              << ":" << ntohs(client_addr.sin_port) << " подключился.\n";

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

        if (bytes_received <= 0) {
            break;
        }
        broadcast(std::string(buffer, bytes_received), client_socket);
    }

    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end());
    }
    close(client_socket);
    std::cout << "Клиент " << inet_ntoa(client_addr.sin_addr) << " отключился.\n";
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Ошибка создания сокета\n";
        return 1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr))) {
        std::cerr << "Ошибка привязки сокета\n";
        return 1;
    }

    if (listen(server_fd, 5) == -1) {
        std::cerr << "Ошибка прослушивания\n";
        return 1;
    }

    std::cout << "Сервер запущен и ожидает подключения...\n";

    while (true) {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_fd, (sockaddr*)&client_addr, &client_len);

        if (client_socket == -1) {
            std::cerr << "Ошибка принятия подключения\n";
            continue;
        }

        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients.push_back(client_socket);
        }

        std::thread([client_socket, client_addr]() {
            handle_client(client_socket, client_addr);
        }).detach();
    }
    close(server_fd);
    return 0;
}
