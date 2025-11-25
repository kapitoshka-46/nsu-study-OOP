#include "mix.h"
#include "../../terminal/color.h"

#include <iostream>
void print_vector_of_strings(std::vector<std::string> &vec) {
    std::cout << color::bold << color::bright_magenta << "files for mixing:\n" << color::reset;
    std::cout << color::magenta;
    for (auto x: vec) {
        std::cout << "\t" << x << "\n";
    }
    std::cout << color::reset;
    std::cout << "\n";
}

converter::Mix::Mix(std::vector<std::string> others) {
    print_vector_of_strings(others);
    std::cout << "const: mix for others\n";
}

converter::Mix::Mix(std::vector<std::string> others, Seconds start) {
    print_vector_of_strings(others);
    std::cout << "const: mix for others from " << start << " to end\n";
}

converter::Mix::Mix(std::vector<std::string> others, Seconds start, Seconds end) {
    print_vector_of_strings(others);
    std::cout << "const: mix for others from " << start << " to " << end << "\n";
}

void converter::Mix::Apply(IAudioIn &input, IAudioOut &output) {
    std::cout << "apply: mix \n";
    uint16_t sample;
    while (input >> sample) {
        output << sample;
    }
}

converter::HelpDescriptor converter::Mix::GetHelpDescriptor() const {
    return HelpDescriptor{"Mix source into stream from start",
    {"source", "start=0"},
    {"mix $2 50", "mix $1"}
    };
}
