#include "Gonderen.h"
#include <iostream>
#include <cstring> 
#include <cerrno>  

Gonderen::Gonderen(const std::string& ip, int port) : Gonderen(ip, static_cast<uint16_t>(port)) {}

Gonderen::Gonderen(const std::string& ip, uint16_t port) {
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0) {
        std::cerr << "Gecersiz adres veya adres desteklenmiyor!\n";
        exit(1);
    }

    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd == INVALID_SOCKET) {
        std::cerr << "Soket hatasi: " << errno << " (" << strerror(errno) << ")\n";
        exit(1);
    }
}

Gonderen::~Gonderen() {
    if (socket_fd != INVALID_SOCKET) {
        close(socket_fd);
    }
}

void Gonderen::send_data(int64_t timestamp, float value) {
    char buffer[sizeof(int64_t) + sizeof(float)];
    memcpy(buffer, &timestamp, sizeof(timestamp));
    memcpy(buffer + sizeof(timestamp), &value, sizeof(value));

    int sent_bytes = sendto(socket_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (sent_bytes == SOCKET_ERROR) {
        std::cerr << "Gönderme hatasi: " << errno << " (" << strerror(errno) << ")\n";
    }
    else {
        std::cout << "Gönderilen: " << timestamp << ", " << value << "\n";
    }
}