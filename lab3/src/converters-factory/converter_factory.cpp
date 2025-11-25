#include "converter_factory.h"

#include "../converters/i_converter.h"
#include "../converters/mix/mix.h"
#include "../converters/mute/mute.h"
#include "../converters/dummy/dummy.h"

std::string filename_associated_with_var(std::string var);

std::vector<std::string> get_associated_files(const std::vector<std::string>& file_numbers, const std::vector<std::string> & filenames) {
    std::vector<std::string> result;
    for (const auto &var : file_numbers) {
        // TODO: may be just store numbers not $0 $1 ???
        int number = std::stoi(var.substr(1));  // assume that var has format like $0 $1 etc..
        try {
            result.push_back(filenames.at(number));
        }
        catch (std::out_of_range &ex) {
            throw;
        }
    }
    return result;
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
        [&input_files](Params params) -> std::unique_ptr<IConverter> { // FIXME don't like capture list ...
            std::vector<std::string> filenames = get_associated_files(params.streams, input_files);
            if (params.streams.empty()) {
                throw std::runtime_error("Need at least 1 stream for mix");
            }

            auto num_timestamps = params.time_stamps.size();
            if (num_timestamps == 0) {
                return std::make_unique<Mix>(filenames);
            }

            if (num_timestamps == 1) {
                auto start = params.time_stamps[0];
                return std::make_unique<Mix>(filenames, start);
            }

            if (num_timestamps == 2) {
                auto start = params.time_stamps[0];
                auto end = params.time_stamps[1];
                return std::make_unique<Mix>(filenames, start, end);
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
