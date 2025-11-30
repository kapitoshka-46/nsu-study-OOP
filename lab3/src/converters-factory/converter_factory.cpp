#include "converter_factory.h"

#include "../converters/i_converter.h"
#include "../converters/mix/mix.h"
#include "../converters/mute/mute.h"
#include "../converters/dummy/dummy.h"


std::vector<int> get_streams_indeces(std::vector<std::string> streams) {
    std::vector<int> res;

    for (const auto &stream : streams) {
        if (stream.empty()) {
            continue;
        }
        if (stream[0] != '$' or stream.length() <= 1) {
            throw std::invalid_argument("invalid stream. should be `$Z`, where Z -- number");
        }
        int stream_index = std::stoi(stream.substr(1));
        res.push_back(stream_index);
    }
    return res;
}

converter::ConverterFactory::ConverterFactory(std::vector<std::string> const & input_files)
{
    // realisations of converters constructors
    RegisterConverter(
        "mute",
        [](const Params &params) -> std::unique_ptr<IConverter> {
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
        });  // end of call

    RegisterConverter(
        "mix",
        [](Params params) -> std::unique_ptr<IConverter> { // FIXME don't like capture list ...
            if (params.streams.empty()) {
                throw std::runtime_error("Need at least 1 stream for mix");
            }

            std::vector<int> vars = get_streams_indeces(params.streams);

            auto num_timestamps = params.time_stamps.size();
            if (num_timestamps == 0) {
                return std::make_unique<Mix>(vars);
            }

            if (num_timestamps == 1) {
                auto start = params.time_stamps[0];
                return std::make_unique<Mix>(vars, start);
            }

            if (num_timestamps == 2) {
                auto start = params.time_stamps[0];
                auto end = params.time_stamps[1];
                return std::make_unique<Mix>(vars, start, end);
            }
            throw std::logic_error("Too many timestamps: " + std::to_string(num_timestamps));
        }
    );

    RegisterConverter(
        "dummy",
        [](const Params &params) -> std::unique_ptr<IConverter> {
            if (!params.time_stamps.empty() or !params.streams.empty()) {
                throw std::invalid_argument("dummy converter shouldn't have params");
            }
            return std::make_unique<Dummy>();
        }
    );
}
