#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

class CaesarCipher {
public:
    CaesarCipher(int shift) : shift_(shift % 26) {}

    std::string encrypt(const std::string &text) {
        return transform(text, shift_);
    }

    std::string decrypt(const std::string &text) {
        return transform(text, -shift_);
    }

private:
    int shift_;

    std::string transform(const std::string &text, int shift) {
        std::string result;
        for (char c : text) {
            if (isalpha(c)) {
                char base = islower(c) ? 'a' : 'A';
                c = (c - base + shift + 26) % 26 + base;
            }
            result += c;
        }
        return result;
    }
};

void receive_messages(int sock, CaesarCipher &cipher) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            std::cout << "Соединение с сервером потеряно.\n";
            break;
        }
        std::string decrypted_message = cipher.decrypt(std::string(buffer));
        std::cout << "Сообщение: " << decrypted_message << std::endl;
    }
}

int main() {
    std::string input_password;
    std::cout << "Введите пароль: \n";
    std::getline(std::cin, input_password);

    const std::string correct_password = "abc3";

    if (input_password != correct_password) {
        std::cout << "Неверный пароль. Завершение работы.\n";
        return 1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Ошибка создания сокета\n";
        return 1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Ошибка подключения\n";
        return 1;
    }

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int bytes_received = recv(sock, buffer, sizeof(buffer), 0);
   
    std::string response(buffer);
    if (response == "FULL") {
        std::cout << "Сервер переполнен. Максимальное количество клиентов достигнуто.\n";
        close(sock);
        return 1;
    }

    CaesarCipher cipher(3); 

    std::thread(receive_messages, sock, std::ref(cipher)).detach();

    std::cout << "Введите сообщения (exit для выхода):\n";
    std::string message;
    while (true) {
        std::getline(std::cin, message);
        if (message == "exit") break;

        std::string encrypted_message = cipher.encrypt(message);
        send(sock, encrypted_message.c_str(), encrypted_message.size(), 0);
    }

    close(sock);
    return 0;
}