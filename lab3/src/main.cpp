#include <iostream>

//#include <bits/this_thread_sleep.h>
#include <chrono>

#include "cfg-parser/lexer.h"
#include "cfg-parser/config_parser.h"
#include "converters-factory/converter_factory.h"

using namespace  std::chrono_literals;



int main() {
    auto converters = cfg::ConfigParser::GetConvertersFromConfig("config.cfg", {"input1.wav", "input2.wav"});
    std::vector<int16_t> pseudo_samples {2, 5, 10};
    for (auto &conv : converters) {
        conv->Apply(pseudo_samples);
    }
}
