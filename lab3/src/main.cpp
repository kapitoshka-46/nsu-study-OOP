#include <iostream>

//#include <bits/this_thread_sleep.h>
#include <chrono>
#include <bits/this_thread_sleep.h>

#include "cfg-parser/lexer.h"
#include "cfg-parser/config_parser.h"
#include "converters-factory/converter_factory.h"
#include "sample-stream/wav/wav_stream.h"
#include "sound-processor/sound_processor.h"

using namespace  std::chrono_literals;
using namespace audio_stream;


int main() {

    std::vector<std::string> files = {"/home/kapiuser/Music/laba/wav/cool.wav", "/home/kapiuser/Music/laba/wav/snoop.wav"};
    std::string config_name = "config.cfg";
    sound_processor::SoundProcessor sp(config_name, files);
    sp.RunPipeline();
    return 0;
}
