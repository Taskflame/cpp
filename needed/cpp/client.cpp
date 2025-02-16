#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void receive_messages(int sock) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            std::cout << "Соединение с сервером потеряно.\n";
            break;
        }
        std::cout << "Сообщение: " << buffer << std::endl;
    }
}

int main() {
    std::string input_password;
    std::cout << "Введите пароль \n";

    std::getline(std::cin, input_password);

    const std::string correct_password = "abc3";

    if (input_password != correct_password) {

        std::cout << "Неверный пароль. Завершение работы.\n";

        return 1;
    }



    char password[ ] = "abc3";

    
   


    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Ошибка создания сокета\n";
        return 1;
    }

     char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int bytes_received = recv(sock, buffer, sizeof(buffer), 0);
    // if (bytes_received <= 0) {
    //     std::cout << "Ошибка соединения с сервером\n";
    //     close(sock);
    //     return 1;
    // }
    
    std::string response(buffer);
    if (response == "FULL") {
        std::cout << "Сервер переполнен. Максимальное количество клиентов достигнуто.\n";
        close(sock);
        return 1;
    }
    // else if (response != "OK") {
    //     std::cout << "Неверный пароль!\n";
    //     close(sock);
    //     return 1;
    // }


    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Ошибка подключения\n";
        return 1;
    }

    std::thread(receive_messages, sock).detach();

    std::cout << "Введите сообщения (exit для выхода):\n";
    std::string message;
    while (true) {
        std::getline(std::cin, message);
        if (message == "exit") break;
        send(sock, message.c_str(), message.size(), 0);
    }

    close(sock);
    return 0;
}