#include "Kullanici.h"
#include "Device.h"
#include "Gonderen.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <csignal> 

std::atomic<bool> terminate_program(false);


void signal_handler(int signal) {
    if (signal == SIGINT) {
        std::cout << "\nSIGINT sinyali alindi. Kapatiliyor...\n";
        terminate_program = true;
    }
}

int main() {
    std::signal(SIGINT, signal_handler);

    int saniye;
    std::cout << "Kac saniye veri uretilsin?";
    std::cin >> saniye;

    uint16_t port = 9000;

    Gonderen gonderen("127.0.0.1", port);
    Device device(0.001f, 5.0f, 1.0f, gonderen);

    Kullanici kullanici(port);

    std::thread uretici([&] {
        while (!terminate_program) {
            device.produce_data();
            if (device.is_stop_requested()) break; 
        }
        });

    std::thread tuketici([&] {
        while (!terminate_program) {
            kullanici.receive_loop();
            if (kullanici.is_stop_requested()) break; 
        }
        });

    if (saniye > 0) {
        std::cout << "Veri " << saniye << " saniye boyunca uretilecek...\n";
        std::this_thread::sleep_for(std::chrono::seconds(saniye));
        std::cout << "Sure doldu. Veri uretimi durduruluyor...\n";
        device.request_stop();     
        kullanici.request_stop();  
        terminate_program = true;  
    }
    else {
        device.request_stop();
        kullanici.request_stop();
    }

    if (uretici.joinable()) {
        uretici.join();
    }

    if (tuketici.joinable()) {
        tuketici.join();
    }

    std::cout << "Program basariyla sonlandirildi.\n";
    return 0;
}
