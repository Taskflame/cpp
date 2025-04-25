#include "client.hpp"
#include <iostream>
#include <stdexcept>
#include <cstring>

void ChatClient::connectToServer(const std::string& ip, int port, const std::string& password) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) throw std::runtime_error("Socket error");

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0)
        throw std::runtime_error("Invalid address");

    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        throw std::runtime_error("Connection failed");

    if (!password.empty()) send(sock, password.c_str(), password.size(), 0);
}

void ChatClient::receiveMessages() {
    char buffer[1024];
    while (true) {
        int bytes = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes <= 0) {
            std::cerr << "Server disconnected\n";
            break;
        }
        std::cout << std::string(buffer, bytes) << std::endl;
    }
}