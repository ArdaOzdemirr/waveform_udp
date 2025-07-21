#pragma once
#include "Waveform.h"
#include <cstdint>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <atomic>

typedef int SOCKET;
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)

class Kullanici {
private:
    SOCKET socket_fd;
    struct sockaddr_in server_addr; 
    std::atomic<bool> stop_requested;

public:
    Kullanici(uint16_t port);
    ~Kullanici();
    void receive_loop();
    void request_stop();
    bool is_stop_requested() const;
};