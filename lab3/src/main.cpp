#include <iostream>

#include "cmd-options/cmd_parser.h"
#include "sound-processor/sound_processor.h"
#include "terminal/color.h"


int main(int argc, char **argv) {
    try {
        sound_processor::SoundProcessor sp(argc, argv);

        sp.RunPipeline();

    }
    catch (std::exception &e) {
        std::cout << color::red << e.what() << color::reset <<"\n";
        return 1;
    }
    return 0;
}
