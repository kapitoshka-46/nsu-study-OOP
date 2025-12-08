#include <filesystem>

#include "sound_processor.h"
#include "../sample-stream/wav/wav_stream.h"
#include "../terminal/color.h"
#include "../cmd-options/cmd_parser.h"

namespace fs = std::filesystem;
using namespace audio_stream;
namespace sound_processor {

    // use tmp directory. remember to move out.wav from it and delete!
    // TODO: use tmp_dir from filesystem library
    // warning! user can have their own dir named `tmp`. and program just delete it, lol
    fs::path prepare_tmp_dir() {
        fs::path work_dir {".temp-sound-processor"};
        fs::remove_all(work_dir);
        fs::create_directory(work_dir);
        return work_dir;
    }

    void save_file_and_create_dirs_if_need(const fs::path & src, const fs::path &dst) {
        fs::create_directories(fs::path(dst).remove_filename());
        fs::copy(src, dst, fs::copy_options::update_existing); // FIXME: обрабатывать случай, если файл уже есть
        std::cout << "Saving file in: " << dst << std::endl;

    }

    SoundProcessor::SoundProcessor(int argc, char **argv) :options_(argc, argv) {
        if (options_.PrintHelpIfRequired(std::cout)) {
            is_running = false;
            return;
        }
        converters_ = cfg::Parser::GetConvertersFromConfig(options_.GetConfigurationFilename());
        total_actions = converters_.size() + 2; // + saving results + creating converters
        print_info("Setup configuration");

    }

    void SoundProcessor::RunPipeline() {
        if (not is_running) {
            return;
        }
        const std::vector<fs::path> &in_files = options_.GetInputFileNames();
        const fs::path &result = options_.GetOutputFilename();
        if (in_files.empty()) {return;}

        fs::path work_dir = prepare_tmp_dir();
        auto tmp_in = fs::path(work_dir).concat("/input.wav");
        auto tmp_out = fs::path{work_dir}.concat("/output.wav");
        fs::copy(in_files[0], tmp_in, fs::copy_options::update_existing);

        WAVContext ctx {tmp_in, in_files};
        for (auto &converter : converters_) {
            print_info("Applying " + converter->GetName());

            WAVStreamOutput audio_out {tmp_out, 44100, 16};
            converter->Apply(ctx, audio_out);

            std::swap(tmp_in, tmp_out);
            ctx.SetMainInputStream(tmp_in); // set to swapped file
        }
        std::swap(tmp_in, tmp_out);   // because we swapped input and output in the end of cycle

        print_info("Saving results");
        save_file_and_create_dirs_if_need(tmp_out, result);

    }

    void SoundProcessor::print_info(const std::string &msg) const {
        static int count = 0;
        std::cout << color::bold << color::blue << ":: " << msg << color::reset <<
                color::gray << " ("<< ++count << "/" << total_actions <<  ")" << color::reset << std::endl;
    }
}
