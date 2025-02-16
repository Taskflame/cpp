void broadcast(const std::string& message, int sender_socket) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (int client : clients) {
        if (client != sender_socket) {
            send(client, message.c_str(), message.size(), 0);
        }
    }
}
