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

        const std::string work_dir = "tmp";

        // use tmp directory. remember to move out.wav from it and delete!
        // TODO: use tmp_dir from filesystem library
        // warning! user can have their own dir named `tmp`. and program just delete it, lol
        fs::remove_all(work_dir);
        fs::create_directory(work_dir);
        fs::path input = work_dir + "/" + fs::path(files[0]).replace_filename("first.wav").filename().string();
        fs::path output = work_dir + "/" + "second.wav";

        fs::copy(files[0], input, fs::copy_options::update_existing);

        for (auto &converter : converters_) {

            WAVStreamInput audio_in {input};
            WAVStreamOutput audio_out {output, 44100, 16};

            std::cout << color::bold << color::blue <<":: Applying " << converter->GetName() << color::reset <<std::endl;
            std::cout << color::bold << color::green << input.filename() << color::reset << " --> " << color::green <<
                    output.filename() << color::reset << std::endl;
            converter->Apply(audio_in, audio_out);

            std::swap(input, output);
        }
        std::cout << color::bold << color::blue <<":: Saving results" << color::reset << std::endl;

        std::swap(input, output);   // because we swapped input <-> output in the end of for-cycle

        fs::path result {output};
        result.replace_filename("result.wav");

        std::cout << color::green << output.filename() << color::reset << " ==> "
                  << color::green << result.filename() << color::reset << std::endl;

        fs::copy(output, result, fs::copy_options::update_existing); // TODO: обрабатывать случай, если файл уже есть
        std::cout << "Result is wrote to: " << color::bold << result << std::endl;

    }
}
