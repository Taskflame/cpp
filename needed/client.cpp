#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
<<<<<<< HEAD
#include <string>

std::string encryptDecrypt(const std::string& input, const std::string& key) { //задаём функцию шифрования xor. Есть ссылка на входную строку и на ключ, используемый для шифровки
    std::string output = input; // копия входной строки, которая будет изменена и возвращена как зашифрованная строка
    for (size_t i = 0; i < input.size(); i++) { // цикл, перебирающий каждый символ входной строки
        output[i] = input[i] ^ key[i % key.size()]; // операция xor между тек. символом и символом ключа.  Индекс символа ключа вычисляется с использованием операции %, чтобы индекс всегда был в пределах допустимых значений для ключа
    }
    return output; // возвращаем тут зашифрованную/дешифрованную строку
}

void receive_messages(int sock) { // определяет функцию, которяая отвечает за получение сообщений от сервера. sock - номер сокета
    char buffer[1024]; // массив для хранения полученных данных 
    while (true) { // б.ц. для постоянного получения сообщений от сервера 
        memset(buffer, 0, sizeof(buffer)); // отчистка буфера обмена перед каджым чтением
        int bytes_received = recv(sock, buffer, sizeof(buffer), 0); // функция recv читает данные из сокета и записывает их в буфер. 1 - номер сокета, 2 - указатель на массив, куда будут записаны данные, 3 - Максимальное количество байт для чтения
        if (bytes_received <= 0) { // если количество байт меньше или равно 0, то цикл прерывается 
            std::cout << "Соединение с сервером потеряно.\n"; 
            break;
        }
        std::string encrypted_message(buffer, bytes_received); // Создается строка из полученных данных. Конструктор строки принимает указатель на массив символов и количество символов
        std::string decrypted_message = encryptDecrypt(encrypted_message, "ключ"); // Расшифровка сообщения с использованием функции XOR (encryptDecrypt) и ключа "ключ"
        std::cout << "Сообщение: " << decrypted_message << std::endl; //вывод дешифрованного сообщения
=======

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
>>>>>>> 10e37a9d540e80c3720732f120e17487369afae7
    }
}

int main() {
<<<<<<< HEAD
    std::string input_password; // Объявляется строковая переменная для хранения введенного пароля.
    std::cout << "Введите пароль: "; // Пользователь вводит пароль через стандартный ввод
    std::getline(std::cin, input_password); // getline - функция из <string> для чтения строки текста из входного потока. input_password - переменная для сохранения пароля.

    const std::string correct_password = "abc3"; // Ключевое слово, указывающее, что значение переменной не может быть изменено после инициализации. Это делает correct_password константой.
    if (input_password != correct_password) { 
        std::cout << "Неверный пароль. Завершение работы.\n";
        return 1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0); // Константа, указывающая на то, что сокет будет использовать домены IPv4-адресов. Константа, указывающая, что будет создан потоковый сокет (TCP). TCP обеспечивает упорядоченную передачу данных. 
    if (sock == -1) {
        std::cerr << "Ошибка создания сокета\n"; // Объект из библиотеки <iostream>, представляющий стандартный поток ошибок. Он обычно используется для вывода сообщений об ошибках.
        return 1;
    }

    sockaddr_in server_addr; // Структура, определенная в заголовочном файле. используется для хранения информации об IP-адресе и номере порта для интернет-сокетов (IPv4).
    server_addr.sin_family = AF_INET; // .sin_family: Член структуры sockaddr_in, который указывает семейство адресов. Константа, указывающая на то, что используется семейство адресов IPv4. Как и при создании сокета, это говорит о том, что мы будем использовать IPv4-адреса.
    server_addr.sin_port = htons(12345); // номер используемого потока
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // .s_addr: Член структуры in_addr, который хранит IP-адрес в виде 32-битного целого числа (в сетевом представлении).

    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) { //connect(): Функция, которая устанавливает соединение с сервером. sock - номер сокета. (sockaddr*)&server_addr: Указатель на структуру sockaddr, содержащую адрес сервера. (sockaddr*) - это приведение типа, которое необходимо, потому что функция connect() ожидает указатель на sockaddr, а не sockaddr_in.
=======
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
>>>>>>> 10e37a9d540e80c3720732f120e17487369afae7
        std::cerr << "Ошибка подключения\n";
        return 1;
    }

<<<<<<< HEAD
    std::thread(receive_messages, sock).detach(); // std::thread - класс, который представляет поток выполнения. receive_messages: Имя функции, которая будет выполняться в новом потоке.
   // Эта функция получает сообщения от сервера. sock: Аргумент, который передается в функцию receive_messages. Это дескриптор сокета. После вызова detach() поток продолжает выполняться независимо от основной программы. Основная программа не будет ждать завершения этого потока.
=======
    std::thread(receive_messages, sock).detach();
>>>>>>> 10e37a9d540e80c3720732f120e17487369afae7

    std::cout << "Введите сообщения (exit для выхода):\n";
    std::string message;
    while (true) {
<<<<<<< HEAD
        std::getline(std::cin, message); // Функция для чтения строки из стандартного ввода. пользователь водит строку и она сохраняется в message
        if (message == "exit") break;
        message = encryptDecrypt(message, "ключ"); // шифровка сообщение
        send(sock, message.c_str(), message.size(), 0); //Преобразование std::string в C-style строку (массив char).

    }

    close(sock); //закрыть данный нам сокет
    return 0;
}
=======
        std::getline(std::cin, message);
        if (message == "exit") break;
        send(sock, message.c_str(), message.size(), 0);
    }

    close(sock);
    return 0;
}
>>>>>>> 10e37a9d540e80c3720732f120e17487369afae7
