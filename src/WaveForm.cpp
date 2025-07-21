#include "Waveform.h"

void Waveform::add_sample(float value) {
    samples.push_back(value);
}

void Waveform::print() const {
    std::cout << "Uretilen " << samples.size() << " veri: ";
    for (auto val : samples) {

        std::cout << val << "    ";
    
    }

    std::cout << "\n";
}

size_t Waveform::size() const {
    return samples.size();
}

void Waveform::clear() {
    samples.clear();
}