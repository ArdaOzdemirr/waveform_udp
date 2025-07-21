#include "Kullanici.h"
#include <iostream>
#include <chrono>
#include <cstring> 
#include <cerrno>  

Kullanici::Kullanici(uint16_t port) : stop_requested(false) {
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd == INVALID_SOCKET) {
        std::cerr << "Soket olusturma hatasi: " << errno << " (" << strerror(errno) << ")\n";
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 

    if (bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Baglama hatasi: " << errno << " (" << strerror(errno) << ")\n";
        if (socket_fd != INVALID_SOCKET) {
            close(socket_fd);
        }
        exit(1);
    }
}

Kullanici::~Kullanici() {
    if (socket_fd != INVALID_SOCKET) {
        close(socket_fd);
    }
}

void Kullanici::receive_loop() {
    char buffer[sizeof(int64_t) + sizeof(float)];
    sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    Waveform wf;

    while (!stop_requested) {
        int recv_bytes = recvfrom(socket_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_addr_len);
        if (recv_bytes == SOCKET_ERROR) {
            if (errno == EINTR) { 
                continue;
            }
            std::cerr << "Veri alma hatasi: " << errno << " (" << strerror(errno) << ")\n";
            continue;
        }

        if (recv_bytes != sizeof(buffer)) {
            std::cerr << "Beklenmeyen sayida bayt alindi: " << recv_bytes << ", beklenen: " << sizeof(buffer) << "\n";
            continue;
        }

        int64_t timestamp;
        float value;
        memcpy(&timestamp, buffer, sizeof(timestamp));
        memcpy(&value, buffer + sizeof(timestamp), sizeof(value));

        auto now = std::chrono::high_resolution_clock::now();
        int64_t now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        auto latency = now_ms - timestamp;

        wf.add_sample(value);
        std::cout << "Alinan: " << timestamp << " , " << value << " (Gecikme: " << latency << "ms)\n";

        if (wf.size() >= 50) {
            wf.print();
            wf.clear();
        }
    }
}

void Kullanici::request_stop() {
    stop_requested = true;
}

bool Kullanici::is_stop_requested() const {
    return stop_requested;
}