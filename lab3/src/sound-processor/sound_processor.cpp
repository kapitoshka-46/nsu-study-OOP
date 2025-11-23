#include "sound_processor.h"
#include "../sample-stream/wav/wav_stream.h"
#include "../terminal/color.h"
#include <filesystem>

namespace fs = std::filesystem;
using namespace audio_stream;
namespace sound_processor {
    SoundProcessor::SoundProcessor(const std::string& config, const std::vector<std::string>& files)
        // get ptrs to all converters that we need in config file
        : files(files),
        // store files for opening them later
        converters_(cfg::Parser::GetConvertersFromConfig(config, files))
    {}

    void SoundProcessor::RunPipeline() const {
        if (files.empty()) {return;}

        const std::string tmp_dir = "tmp";

        // use tmp directory. remember to move out.wav from it and delete!
        fs::create_directory(tmp_dir);
        std::string input = tmp_dir + "/" + fs::path(files[0]).filename().string();
        std::string output = tmp_dir + "/" + "out.wav";

        fs::copy(files[0], input, fs::copy_options::update_existing);

        for (auto &converter : converters_) {

            WAVStreamInput audio_in {input};
            WAVStreamOutput audio_out {output, 44100, 16};

            std::cout << color::bold  << color::blue <<":: Applying " << converter->GetName() << color::reset <<std::endl;
            converter->Apply(audio_in, audio_out);
            audio_in.Rewind();
            audio_out.Rewind();

            std::swap(input, output);
        }



    }
}
