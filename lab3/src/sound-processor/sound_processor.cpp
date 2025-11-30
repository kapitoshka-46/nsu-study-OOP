#include "sound_processor.h"
#include "../sample-stream/wav/wav_stream.h"
#include "../terminal/color.h"
#include <filesystem>

namespace fs = std::filesystem;
using namespace audio_stream;
namespace sound_processor {



    SoundProcessor::SoundProcessor(const std::string& config, const std::vector<std::string>& files)
        // get ptrs to all converters that we need in config file
        : input_files(files),
        // store files for opening them later
        converters_(cfg::Parser::GetConvertersFromConfig(config, files))
    {}

    void SoundProcessor::RunPipeline() const {
        if (input_files.empty()) {return;}

        const std::string work_dir = "tmp";

        // use tmp directory. remember to move out.wav from it and delete!
        // TODO: use tmp_dir from filesystem library
        // warning! user can have their own dir named `tmp`. and program just delete it, lol
        fs::remove_all(work_dir);
        fs::create_directory(work_dir);
        fs::path input_path = work_dir + "/" + fs::path(input_files[0]).replace_filename("first.wav").filename().string();
        fs::path output_path = work_dir + "/" + "second.wav";

        fs::copy(input_files[0], input_path, fs::copy_options::update_existing);


        WAVContext ctx {input_path, input_files};

        // file1 -> converter1 -> file2 -> converter2 -> file1 -> ... ->
        for (auto &converter : converters_) {
            WAVStreamOutput audio_out {output_path, 44100, 16};

            std::cout << color::bold << color::blue <<":: Applying " << converter->GetName() << color::reset <<std::endl;
            std::cout << color::bold << color::green << input_path.filename() << color::reset << " --> " << color::green <<
                    output_path.filename() << color::reset << std::endl;
            converter->Apply(ctx, audio_out);

            std::swap(input_path, output_path); //
            ctx.SetMainInputStream(input_path); // set to swapped file
        }
        std::cout << color::bold << color::blue <<":: Saving results" << color::reset << std::endl;

        std::swap(input_path, output_path);   // because we swapped input <-> output in the end of for-cycle

        fs::path result {output_path};
        result.replace_filename("result.wav");

        std::cout << color::green << output_path.filename() << color::reset << " ==> "
                  << color::green << result.filename() << color::reset << std::endl;

        fs::copy(output_path, result, fs::copy_options::update_existing); // TODO: обрабатывать случай, если файл уже есть
        std::cout << "Result is wrote to: " << color::bold << result << std::endl;

    }
}
