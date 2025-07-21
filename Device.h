#pragma once
#include "Gonderen.h"
#include <atomic>

class Device {
private:

    float dt, frekans, genlik;
    std::atomic<bool> stop_requested;
    Gonderen& gonderen;

public:

    Device(float dt, float frek, float gen, Gonderen& gonderen);
    void produce_data();
    void request_stop();
    bool is_stop_requested() const;

};