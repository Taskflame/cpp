#include "server.hpp"
#include <iostream>
#include <algorithm>

class BasicParticipant : public ChatParticipant {
public:
    BasicParticipant(ChatServer* srv, int sock, const std::string& addr) 
        : ChatParticipant(srv, sock, addr, "user") {}

    void handle() override {
        char buffer[1024];
        while (true) {
            int bytes = recv(socket, buffer, sizeof(buffer), 0);
            if (bytes <= 0) break;
            server->broadcast(std::string(buffer, bytes), socket); // Вызываем через server->
        }
    }
};

ChatParticipant::ChatParticipant(ChatServer* srv, int sock, const std::string& addr, const std::string& r) 
    : server(srv), socket(sock), address(addr), role(r) {}

ChatParticipant::~ChatParticipant() {
    if (socket != -1) close(socket);
}

ChatServer::ChatServer(int port) {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) throw std::runtime_error("Socket error");

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
        throw std::runtime_error("Bind failed");

    if (listen(server_fd, 5) < 0) throw std::runtime_error("Listen failed");
}

ChatServer::~ChatServer() {
    for (auto p : participants) delete p;
    close(server_fd);
}

void ChatServer::broadcast(const std::string& message, int sender) {
    std::lock_guard<std::mutex> lock(participants_mutex);
    for (auto p : participants) {
        if (p->getSocket() != sender && p->getSocket() != -1) {
            send(p->getSocket(), encrypt(message).c_str(), message.size(), 0);
        }
    }
}

void ChatServer::run() {
    while (true) {
        sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        int client_sock = accept(server_fd, (sockaddr*)&client_addr, &len);
        if (client_sock < 0) continue;

        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, ip, INET_ADDRSTRLEN);

        std::lock_guard<std::mutex> lock(participants_mutex);
        participants.push_back(new BasicParticipant(this, client_sock, ip)); // Передаем this
        std::thread(&ChatParticipant::handle, participants.back()).detach();
    }
}

void ChatServer::regAdmin(const std::string& name, const std::string& pass) {
    admin_credentials[name] = pass;
}