#include "mute.h"

converter::Mute::Mute(Seconds start) {
    std::cout << "const: mute from " << start << " to end" << "\n";
}

converter::Mute::Mute(Seconds start, Seconds end) {
    std::cout << "const: mute from " << start << " to " << end << "\n";
}

void converter::Mute::Apply(std::vector<int16_t> &samples) {
    std::cout << "apply: mute\n";
}
