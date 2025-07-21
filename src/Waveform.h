#pragma once
#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <vector>
#include <iostream>

class Waveform {
private:
    std::vector<float> samples;

public:
    void add_sample(float value);
    void print() const;
    size_t size() const;
    void clear();
};

#endif 