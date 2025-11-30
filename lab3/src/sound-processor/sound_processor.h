#ifndef SOUND_PROCESSOR_H
#define SOUND_PROCESSOR_H
#include <string>
#include <vector>
#include "../cfg-parser/config_parser.h"
#include "../converters-factory/converter_factory.h"

namespace sound_processor {

    class SoundProcessor {

    public:
        SoundProcessor(const std::string& config, const std::vector<std::string>& files);

        void RunPipeline() const;

    private:
        const std::vector<std::string> &input_files;
        cfg::PointersToConverters converters_;
    };


}



#endif //SOUND_PROCESSOR_H
