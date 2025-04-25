#include "server.hpp"
#include <iostream>

int main() {
    try {
        ChatServer server(12345);
        server.regAdmin("admin", "admin123");
        std::cout << "Server started on port 12345\n";
        server.run();
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}