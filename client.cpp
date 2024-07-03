#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void sendPeriodicMessages(const std::string& clientName, int port, int period) {
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address / Address not supported" << std::endl;
        return;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        return;
    }

    while (true) {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;

        std::stringstream message;
        message << "[" << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S")
                << "." << std::setfill('0') << std::setw(3) << milliseconds.count() << "] "
                << "\"" << clientName << "\"";

        send(sock, message.str().c_str(), message.str().length(), 0);
        std::this_thread::sleep_for(std::chrono::seconds(period));
    }

    close(sock);
}

int main(int argc, char const* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: client <client_name> <port> <period>" << std::endl;
        return 1;
    }

    std::string clientName = argv[1];
    int port = std::stoi(argv[2]);
    int period = std::stoi(argv[3]);

    sendPeriodicMessages(clientName, port, period);

    return 0;
}
