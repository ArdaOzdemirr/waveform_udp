#pragma once
#include <cstdint>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> 

typedef int SOCKET;
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)

class Gonderen {
private:
    SOCKET socket_fd;
    struct sockaddr_in server_addr;

public:
    Gonderen(const std::string& ip, int port);
    Gonderen(const std::string& ip, uint16_t port);
    ~Gonderen();
    void send_data(int64_t timestamp, float value);
};