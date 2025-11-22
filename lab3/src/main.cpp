#include <iostream>

//#include <bits/this_thread_sleep.h>
#include <chrono>
#include <bits/this_thread_sleep.h>

#include "cfg-parser/lexer.h"
#include "cfg-parser/config_parser.h"
#include "converters-factory/converter_factory.h"
#include "sample-stream/wav/wav_stream.h"

using namespace  std::chrono_literals;
using namespace sample_stream;


int main() {
    // auto converters = cfg::ConfigParser::GetConvertersFromConfig("config.cfg", {"input1.wav", "input2.wav"});
    // std::vector<int16_t> pseudo_samples {2, 5, 10};
    // for (auto &conv : converters) {
    //     conv->Apply(pseudo_samples);
    // }
    ISampleStreamInput* ssi = new WAVStreamInput("/home/kapiuser/Music/laba/wav/klub.wav");
    delete ssi;
    std::ifstream in;
    return 0;
}
