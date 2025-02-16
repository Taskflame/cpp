#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>

//задаём функцию шифрования xor. Есть ссылка на входную строку и на ключ, используемый для шифровки
std::string encryptDecrypt(const std::string& input, const std::string& key) { 
    std::string output = input; // копия входной строки, которая будет изменена и возвращена как зашифрованная строка
    for (size_t i = 0; i < input.size(); i++) { // цикл, перебирающий каждый символ входной строки
        // операция xor между тек. символом и символом ключа. Индекс символа ключа вычисляется с использованием операции %
        output[i] = input[i] ^ key[i % key.size()]; 
    }
    return output; // возвращаем тут зашифрованную/дешифрованную строку
}

// определяет функцию, которая отвечает за получение сообщений от сервера. sock - номер сокета
void receive_messages(int sock) { 
    char buffer[1024]; // массив для хранения полученных данных 
    while (true) { // б.ц. для постоянного получения сообщений от сервера 
        memset(buffer, 0, sizeof(buffer)); // очистка буфера обмена перед каждым чтением
        // функция recv читает данные из сокета и записывает их в буфер:
        // 1 - номер сокета, 2 - указатель на массив, куда будут записаны данные, 3 - Максимальное количество байт для чтения
        int bytes_received = recv(sock, buffer, sizeof(buffer), 0); 
        if (bytes_received <= 0) { // если количество байт меньше или равно 0, то цикл прерывается 
            std::cout << "Соединение с сервером потеряно.\n"; 
            break;
        }
        // Создается строка из полученных данных. Конструктор строки принимает указатель на массив символов и количество символов
        std::string encrypted_message(buffer, bytes_received);
        // Расшифровка сообщения с использованием функции XOR (encryptDecrypt) и ключа "ключ"
        std::string decrypted_message = encryptDecrypt(encrypted_message, "ключ");
        std::cout << "Сообщение: " << decrypted_message << std::endl; // вывод дешифрованного сообщения
    }
}

int main() {
    std::string input_password; // Объявляется строковая переменная для хранения введенного пароля.
    std::cout << "Введите пароль: "; // Пользователь вводит пароль через стандартный ввод
    std::getline(std::cin, input_password); // функция из <string> для чтения строки текста из входного потока

    // Ключевое слово, указывающее, что значение переменной не может быть изменено после инициализации
    const std::string correct_password = "abc3"; 
    if (input_password != correct_password) { 
        std::cout << "Неверный пароль. Завершение работы.\n";
        return 1;
    }

    // Константа, указывающая на то, что сокет будет использовать домены IPv4-адресов
    // SOCK_STREAM - потоковый сокет (TCP), 0 - протокол по умолчанию
    int sock = socket(AF_INET, SOCK_STREAM, 0); 
    if (sock == -1) {
        std::cerr << "Ошибка создания сокета\n"; // вывод сообщения об ошибке
        return 1;
    }

    // Структура для хранения информации об адресе сервера
    sockaddr_in server_addr; 
    server_addr.sin_family = AF_INET; // Семейство адресов IPv4
    server_addr.sin_port = htons(12345); // Порт сервера в сетевом порядке байтов
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Локальный IP-адрес

    // Установка соединения с сервером
    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) { 
        std::cerr << "Ошибка подключения\n";
        return 1;
    }

    // Проверка ответа сервера
    char server_response[1024];
    memset(server_response, 0, sizeof(server_response));
    if (recv(sock, server_response, sizeof(server_response), 0) <= 0) {
        std::cerr << "Ошибка при получении ответа сервера\n";
        close(sock);
        return 1;
    }
    
    if (std::string(server_response) == "FULL") {
        std::cout << "Сервер переполнен. Попробуйте позже.\n";
        close(sock);
        return 1;
    }

    // Запуск потока для приема сообщений
    std::thread(receive_messages, sock).detach();

    // Основной цикл отправки сообщений
    std::cout << "Введите сообщения (exit для выхода):\n";
    std::string message;
    while (true) {
        std::getline(std::cin, message); // Чтение сообщения от пользователя
        if (message == "exit") break;
        message = encryptDecrypt(message, "ключ"); // Шифрование сообщения
        send(sock, message.c_str(), message.size(), 0); // Отправка сообщения
    }

    close(sock); // Закрытие сокета
    return 0;
}
