#ifndef SOUND_PROCESSOR_H
#define SOUND_PROCESSOR_H
#include <filesystem>
#include <vector>
#include "../cfg-parser/config_parser.h"
#include "../cmd-options/cmd_parser.h"


namespace sound_processor {
    namespace  fs = std::filesystem;

    class SoundProcessor {

    public:
        SoundProcessor(int argc,char **argv);
        void RunPipeline();
    private:
        const opt::Options options_;
        cfg::PointersToConverters converters_{};
        int total_actions;

        void print_info(const std::string &msg) const;
    };


}



#endif //SOUND_PROCESSOR_H
