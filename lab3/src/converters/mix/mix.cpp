#include "mix.h"

#include <iostream>

void print_vector(std::vector<std::string> &vec) {
    for (auto x: vec) {
        std::cout << x << " ";
    }
    std::cout << "\n";
}
converter::Mix::Mix(std::vector<std::string> others) {
    std::cout << "const: mix for others\n";
}

converter::Mix::Mix(std::vector<std::string> others, Seconds start) {
    std::cout << "const: mix for others from " << start << " to end\n";
}

converter::Mix::Mix(std::vector<std::string> others, Seconds start, Seconds end) {
    std::cout << "const: mix for others from " << start << " to " << end << "\n";
}

void converter::Mix::Apply(IAudioIn &input, IAudioOut &output) {
    std::cout << "apply: mix\n";
}
