#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

class ChatServer;

class ChatParticipant {
protected:
    ChatServer* server;
    int socket;
    std::string role;
    std::string address;

public:
    ChatParticipant(ChatServer* srv, int sock, const std::string& addr, const std::string& r);
    virtual ~ChatParticipant();
    virtual void handle() = 0;
    std::string getRole() const { return role; }
    int getSocket() const { return socket; }
    std::string getAddress() const { return address; }
};

class ChatServer {
private:
    int server_fd;
    sockaddr_in server_addr;
    std::vector<ChatParticipant*> participants;
    std::mutex participants_mutex;
    std::unordered_map<std::string, std::string> admin_credentials;

    static std::string encrypt(const std::string& message) {
        std::string result = message;
        for (char& c : result) c ^= 'K';
        return result;
    }

public:
    explicit ChatServer(int port);
    ~ChatServer();
    void broadcast(const std::string& message, int sender_socket);
    void run();
    void regAdmin(const std::string& name, const std::string& password);
};

