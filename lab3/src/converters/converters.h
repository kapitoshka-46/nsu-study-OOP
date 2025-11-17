#ifndef CONVERTERS_H
#define CONVERTERS_H
#include <array>
#include <chrono>
#include <iostream>

#include "iconverter.h"

namespace converter {

    class Mute : IConverter {
        explicit Mute(ConverterParams params) {
            std::cout << "mute" << "\n";
        }
    };

    class Min : IConverter {
        explicit Min(ConverterParams params) {
            std::cout << "min" << "\n";
        }
    };
}

#endif //CONVERTERS_H
