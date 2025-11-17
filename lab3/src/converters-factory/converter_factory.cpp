#include "converter_factory.h"

#include "../converters/mix/mix.h"
#include "../converters/mute/mute.h"

converter::ConverterFactory::ConverterFactory(std::vector<std::string> const & input_files)
    : input_files(input_files)
{
    // realisations of converters constructors
    RegisterConverter(
        "mute",
        [](Params params) {
            auto size = params.time_stamps.size();
            if (size == 1) {
                auto start = params.time_stamps[0];
                return std::make_unique<Mute>(start);
            }
            else if (size == 2) {
                auto start = params.time_stamps[0];
                auto end = params.time_stamps[1];
                return std::make_unique<Mute>(start, end);
            }
            else  {
                throw std::runtime_error("Invalid timestamp size: " + std::to_string(size));
            }
        },
    {"Mutes from start to end",
        {"start", "end"},
        {"mute 20 50", "mute 10"}});  // end of call

    RegisterConverter(
        "mix",
        [input_files](Params params) {
            if (params.streams.empty()) {
                throw std::runtime_error("Need at least 1 stream for mix");
            }
            std::vector<std::string> others;
            for (auto i: params.streams) {
                if (i >= input_files.size()) {
                    throw std::runtime_error("Not enough files for stream $" + std::to_string(i));
                }
                others.push_back(input_files[i]);
            }
            auto num_timestamps = params.time_stamps.size();
            if (num_timestamps == 0) {
                return  std::make_unique<Mix>(others);

            }
            if (num_timestamps == 1) {
                auto start = params.time_stamps[0];
                return std::make_unique<Mix>(others, start);
            }
            if (num_timestamps == 2) {
                auto start = params.time_stamps[0];
                auto end = params.time_stamps[1];
                return std::make_unique<Mix>(others, start, end);
            }
            throw std::logic_error("Too many timestamps (" + std::to_string(num_timestamps) + ")");
    },
        {"Mixing with other files",
        {"streams...", "start", "end"},
        {"mix $0", "mix $1 20 30"}}
    );
}
