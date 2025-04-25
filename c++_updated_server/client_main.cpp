#include "client.hpp"
#include <iostream>
#include <thread>

int main() {
    try {
        ChatClient client;
        std::cout << "Connect as (1) user or (2) admin? ";
        std::string input;
        std::getline(std::cin, input);

        if (input == "2") {
            std::string password;
            std::cout << "Enter admin password: ";
            std::getline(std::cin, password);
            client.connectToServer("127.0.0.1", 12345, password);
        } else {
            client.connectToServer("127.0.0.1", 12345);
        }

        std::thread receive_thread(&ChatClient::receiveMessages, &client);
        
        std::string message;
        while (std::getline(std::cin, message) && message != "exit") {
            client.sendMessage(message);
        }

        receive_thread.detach();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}