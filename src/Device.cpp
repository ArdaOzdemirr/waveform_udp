#include "Device.h"
#include <cmath>
#include <chrono>
#include <thread>
#include <iostream> 

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

Device::Device(float dt, float frek, float gen, Gonderen& gonderen)
    : dt(dt), frekans(frek), genlik(gen), stop_requested(false), gonderen(gonderen) {
}

void Device::produce_data() {
    size_t sample_count = 0;
    while (!stop_requested) {
        auto now = std::chrono::high_resolution_clock::now();
        int64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        float t = sample_count * dt;
        float value = genlik * sinf(2 * M_PI * frekans * t);
        gonderen.send_data(timestamp, value);
        sample_count++;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void Device::request_stop() {
    stop_requested = true;
}

bool Device::is_stop_requested() const {
    return stop_requested;
}