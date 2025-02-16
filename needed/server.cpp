<<<<<<< HEAD
// описание кода сервера цже есть на гите в прошлом коммите
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

std::vector<int> clients;
std::mutex clients_mutex;

void broadcast(const std::string& message, int sender_socket) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (int client : clients) {
        if (client != sender_socket) {
            send(client, message.c_str(), message.size(), 0);
=======
// Файловый дескриптор клиентского сокета — это целочисленный идентификатор, который операционная система присваивает сокету, созданному для взаимодействия с конкретным клиентом
// Вектор - это динамический массив, который может изменять свой размер во время выполнения программы
#include <iostream>
#include <vector>
#include <thread> // библиотека для создания и управления потоками. Потоки позволяют выполнять несколько задач параллельно в одной программе.
#include <mutex> // библиотека для подключения и работы с мьютексами, которые используются для решения проблемы гонки, которая возникает, когда несколько потоков или процессов пытаются одновременно получить доступ к общему ресурсу. В нашем случае - к вектору клиентов.
#include <algorithm> // подключает библиотеку algorithm, которая содержит различные алгоритмы, для работы с контейнерами, такие как сортировка, поиск, удаление. Нужный нам алгоритм "remove"
#include <cstring> // подключает библиотеку cstring, которая предоставляет функции для работы со строками в стиле C. Нужный нам метод "memset" заполняет указанное количество байтов блока памяти заданным значением  
#include <unistd.h> // различные системные вызовы POSIX (например, close для закрытия файлового дескриптора, то есть сокетов)
#include <arpa/inet.h> // содержит функции для преобразования IP-адресов между строковым и числовым представлением 
#include <sys/socket.h> // подключает библиотеку sys/socket.h, которая содержит структуры и функции для работы с сокетами (например, socket, bind, listen, accept, send, recv)

std::vector<int> clients; // храним сокеты подключенных клиентов
std::mutex clients_mutex; // синхронизация доступа к вектору "clients". Проще говоря, нам это надо для того, чтобы сразу 2 хоста не могли одновременно работать с "базой" клиентов

void broadcast(const std::string& message, int sender_socket) { // функция broadcast отправляет сообщение всем клиентам, кроме отправителя. Принимаем на вход ссылку на сообщение, написанное клиентом и сокет отправителя 
    std::lock_guard<std::mutex> lock(clients_mutex); // в этой строчке мы захватываем мьютекс при создании и освобождает при уничтожении объекта. lock - заблокировать
    for (int client : clients) { // перебирает все присвоенные сокетам номера клиентов
        if (client != sender_socket) { // проверяет, является ли текущий клиент отправителем сообщения. Если нет, то сообщение отправляется этому клиенту.
            send(client, message.c_str(), message.size(), 0); // отправляет сообщение клиенту с (файловым дескриптором)(номером сокета) client. message.c_str() возвращает указатель на строку в стиле C, message.size() возвращает размер сообщения в байтах, а 0 указывает на отсутствие дополнительных флагов.
>>>>>>> 10e37a9d540e80c3720732f120e17487369afae7
        }
    }
}

<<<<<<< HEAD
void handle_client(int client_socket, sockaddr_in client_addr) {
    char buffer[1024];
    std::cout << "Клиент " << inet_ntoa(client_addr.sin_addr)
              << ":" << ntohs(client_addr.sin_port) << " подключился.\n";

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

        if (bytes_received <= 0) {
            break;
        }
        broadcast(std::string(buffer, bytes_received), client_socket);
    }

    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end());
    }
    close(client_socket);
    std::cout << "Клиент " << inet_ntoa(client_addr.sin_addr) << " отключился.\n";
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
=======
void handle_client(int client_socket, sockaddr_in client_addr) { // функция, которая принимает целочисленный номер сокета текущего клиента и структуру sockaddr_in, который содержит ip adress слиента (host)
    char buffer[1024]; // создаём масссив символов для хранения сообщений клиента 
    std::cout << "Клиент " << inet_ntoa(client_addr.sin_addr) // преобразует IP-адрес клиента из числового представления в строковое представление
              << ":" << ntohs(client_addr.sin_port) << " подключился.\n"; // Преобразует номер порта клиента из сетевого порядка байтов в порядок байтов хоста

    while (true) { // Это начало бесконечного цикла while, который будет выполняться до тех пор, пока клиент не отключится.
        memset(buffer, 0, sizeof(buffer)); // Эта функция заполняет буфер обмена сообщениями buffer нулями. buffer: Указатель на буфер, который нужно заполнить. 0: Значение, которым нужно заполнить буфер (в данном случае, нуль). sizeof(buffer): Размер буфера в байтах. Это делается для очистки буфера от данных, оставшихся от предыдущего приема.
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0); // Эта функция принимает данные от клиента через сокет client_socket и сохраняет их в буфере buffer. client_socket: Файловый дескриптор сокета клиента, из которого принимаются данные. buffer: Указатель на буфер, в который сохраняются принятые данные. sizeof(buffer): Максимальное количество байтов, которое можно принять.

        if (bytes_received <= 0) { // Это условный оператор, который проверяет, было ли соединение закрыто или произошла ли ошибка при приеме данных.
            break; // в случае отключения клиента или ошибки - завершаем цикл 
        }
        std::cout << "Сообщение от " << inet_ntoa(client_addr.sin_addr) // Преобразует IP-адрес клиента из числового представления в строковое представление
                  << ": " << buffer << std::endl; // Буфер, содержащий сообщение от клиента
        broadcast(std::string(buffer), client_socket); // Эта функция вызывает функцию broadcast для отправки полученного сообщения всем остальным подключенным клиентам. std::string(buffer): Создает объект std::string из буфера buffer. client_socket: Файловый дескриптор сокета клиента, отправившего сообщение.
 
    }

    {
        std::lock_guard<std::mutex> lock(clients_mutex); // Создает объект lock, который захватывает мьютекс clients_mutex при создании и освобождает его при уничтожении. Это обеспечивает безопасный доступ к вектору clients из разных потоков.
        clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end()); // Эта строка удаляет присвоенный компьютером номер сокета клиента (client_socket) из вектора clients. Перемещаем все элементы ветора clients, н равные текущему клиенту (client_socket), в начало вектора. озвращает итератор на первый элемент, который нужно удалить. clients.erase(..., clients.end()): Удаляет элементы из вектора clients от итератора, возвращенного std::remove, до конца вектора.
    }
    close(client_socket); // Эта функция закрывает сокет клиента, освобождая файловый дескриптор
    std::cout << "Клиент " << inet_ntoa(client_addr.sin_addr) << " отключился.\n"; // std::cout << "Клиент " << inet_ntoa(client_addr.sin_addr) << " отключился.\n";: Эта строка выводит сообщение в консоль об отключении клиента.
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0); // Эта функция создает сокет (socket) для сервера. AF_INET - используем IPv4. SOCK_STREAM - Указывает, что будет использоваться потоковый сокет (TCP). 0: Указывает на использование протокола по умолчанию для данного типа сокета (TCP). server_fd: Файловый дескриптор сокета, который будет использоваться для приема входящих соединений. Если socket возвращает -1, это означает ошибку при создании сокета. 
>>>>>>> 10e37a9d540e80c3720732f120e17487369afae7
    if (server_fd == -1) {
        std::cerr << "Ошибка создания сокета\n";
        return 1;
    }

<<<<<<< HEAD
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr))) {
        std::cerr << "Ошибка привязки сокета\n";
        return 1;
    }

=======
    sockaddr_in server_addr; // sockaddr_in server_addr;: Это объявление переменной server_addr, которая представляет собой структуру sockaddr_in. Эта структура будет использоваться для хранения адреса сервера
    server_addr.sin_family = AF_INET; // Эта строка устанавливает поле sin_family структуры server_addr в значение AF_INET, что указывает на использование IPv4
    server_addr.sin_port = htons(12345); // Эта строка устанавливает поле sin_port структуры server_addr в значение 12345, что указывает на номер порта, который будет прослушивать сервер. Функция htons преобразует порядок байтов порта из порядка хоста в сетевой порядок.
    server_addr.sin_addr.s_addr = INADDR_ANY; //  Эта строка устанавливает поле sin_addr.s_addr структуры server_addr в значение INADDR_ANY, что указывает на то, что сервер будет принимать соединения на всех доступных IP-адресах на машине.
// Это условный оператор, который проверяет, произошла ли ошибка при привязке сокета server_fd к адресу server_addr. Если bind возвращает -1, то выводится сообщение об ошибке в поток std::cerr и программа завершается с кодом завершения 1
    if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Ошибка привязки сокета\n";
        return 1;
    }
// Это условный оператор, который проверяет, произошла ли ошибка при начале прослушивания сокета server_fd на предмет входящих соединений. Аргумент 5 указывает максимальную длину очереди ожидающих соединений. Если listen возвращает -1, то выводится сообщение об ошибке в поток std::cerr и программа завершается с кодом завершения 1
>>>>>>> 10e37a9d540e80c3720732f120e17487369afae7
    if (listen(server_fd, 5) == -1) {
        std::cerr << "Ошибка прослушивания\n";
        return 1;
    }

    std::cout << "Сервер запущен и ожидает подключения...\n";

<<<<<<< HEAD
    while (true) {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_fd, (sockaddr*)&client_addr, &client_len);

=======
    while (true) { // Это начало бесконечного цикла while, в котором сервер принимает входящие соединения.
        sockaddr_in client_addr; // Создает структуру sockaddr_in для хранения адреса клиента
        socklen_t client_len = sizeof(client_addr); // Создает переменную client_len для хранения размера структуры client_addr
        int client_socket = accept(server_fd, (sockaddr*)&client_addr, &client_len); // int client_socket = accept(server_fd, (sockaddr*)&client_addr, &client_len);: Принимает входящее соединение на сокете server_fd. Функция accept возвращает новый файловый дескриптор (номер сокета) для соединения с клиентом. Если accept возвращает -1, это означает ошибку при принятии соединения.
        
>>>>>>> 10e37a9d540e80c3720732f120e17487369afae7
        if (client_socket == -1) {
            std::cerr << "Ошибка принятия подключения\n";
            continue;
        }
<<<<<<< HEAD

        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients.push_back(client_socket);
        }

        std::thread([client_socket, client_addr]() {
            handle_client(client_socket, client_addr);
        }).detach();
    }
    close(server_fd);
=======
// Это условный оператор, который проверяет, произошла ли ошибка при принятии соединения. Если client_socket равен -1, то выводится сообщение об ошибке в поток std::cerr и программа переходит к следующей итерации цикла с помощью оператора continue.
        {
            std::lock_guard<std::mutex> lock(clients_mutex); // Создает объект lock, который захватывает мьютекс clients_mutex при создании и освобождает его при уничтожении. Это обеспечивает безопасный доступ к вектору clients из разных потоков.
            clients.push_back(client_socket); // Добавляет номер сокета клиента (client_socket) в вектор clients
        }

        std::thread(handle_client, client_socket, client_addr).detach(); //std::thread(handle_client, client_socket, client_addr): Создает объект std::thread, который запускает функцию handle_client в новом потоке. Аргументы client_socket и client_addr передаются в функцию handle_client. detach(): Отсоединяет поток от родительского потока (в данном случае, от основного потока main), позволяя ему выполняться независимо.
    }

    close(server_fd); // Эта функция закрывает сокет сервера. Этот код никогда не будет выполнен, так как сервер находится в бесконечном цикле. Однако, его можно использовать для завершения работы сервера по сигналу или при возникновении ошибки.
>>>>>>> 10e37a9d540e80c3720732f120e17487369afae7
    return 0;
}