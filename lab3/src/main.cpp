#include <iostream>

//#include <bits/this_thread_sleep.h>
#include <chrono>

#include "cmd-options/cmd_parser.h"
#include "cfg-parser/config_parser.h"
#include "sample-stream/wav/wav_stream.h"
#include "sound-processor/sound_processor.h"

using namespace  std::chrono_literals;
using namespace audio_stream;


int main(int argc, char **argv) {
    cmd::CmdParser parser {argc, argv};
    //parser.PrintHelpIfRequired(std::cout);

    std::vector<std::string> files = {"/home/kapiuser/GitHub/nsu-study-OOP/lab3/music/wav/cool.wav", "/home/kapiuser/GitHub/nsu-study-OOP/lab3/music/wav/snoop.wav"};
    std::string config_name = "config.cfg";
    sound_processor::SoundProcessor sp(config_name, files);
    sp.RunPipeline();
    return 0;
}
