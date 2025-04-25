#pragma once
#include <string>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

class ChatClient {
private:
    int sock = -1;
    std::string role;
    
    static std::string encrypt(const std::string& message) {
        std::string result = message;
        for (char& c : result) c ^= 'K';
        return result;
    }
    
public:
    ~ChatClient() { if (sock != -1) close(sock); }
    void connectToServer(const std::string& ip, int port, const std::string& password = "");
    void receiveMessages();
    void sendMessage(const std::string& message) {
        send(sock, encrypt(message).c_str(), message.size(), 0);
    }
};