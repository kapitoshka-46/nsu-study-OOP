#include "mix.h"

#include <iostream>
#include <limits>
#include <utility>

#include "../../converters-factory/converter_factory.h"


using audio_stream::IntSample;
using namespace converter;

static AutoRegisterConverter r{"mix", std::make_shared<MixCreator>()};


// extracts the number from var.  [$1, $2, ..., $n] -> [1, 2, ..., n]:
static std::vector<int> get_streams_indexes(const std::vector<std::string>& streams) {
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

static void print_input_sources(std::vector<int> &vec) {
    if (vec.size() >= 1) {
        std::cout << "$" << vec[0];
    }
    for (int i = 1; i < vec.size(); i++) {
        std::cout << " $" << vec[i];
    }

    std::cout <<'\n';

}

Mix::Mix(std::vector<int> others) : others(std::move(others)) {}

Mix::Mix(std::vector<int> others, Seconds start) : start_seconds(start), others(std::move(others)) {
    if (start < 0) {throw std::invalid_argument("mix: start cannot be < 0");}
}

Mix::Mix(std::vector<int> others, Seconds start, Seconds end)
: others(std::move(others)),
start_seconds(start),
end_seconds(end) {
    if (start < 0) {throw std::invalid_argument("mix: start cannot be < 0");}
    if (end < start) {throw std::invalid_argument("mix: end cannot be < start");}
}


IntSample mix_samples(const std::vector<IntSample> &samples) {
    int32_t result = 0;
    auto n = samples.size();
    for (IntSample sample : samples) {

        result += static_cast<IntSample>(sample / n);
        constexpr int max = std::numeric_limits<IntSample>::max();
        constexpr int min = std::numeric_limits<IntSample>::min();
        if (result >= max) {
            result = max;
        }
        if (result <= min) {
            result = min;
        }
    }
    return static_cast<int16_t>(result);
}

void Mix::Apply(audio_stream::Context & input_ctx, IAudioOut &output) {
    std::cout << "Mixing with: "; print_input_sources(others);
    IAudioIn &input = input_ctx.GetMainInputStream();

    const uint32_t start_sample = start_seconds * input.GetSampleRate();
    uint32_t end_sample = end_seconds * input.GetSampleRate();
    if (end_seconds == Unset) {
        end_sample = Unset;
    }

    IntSample sample;
    std::vector<IntSample> all_samples;
    for (auto cnt = start_sample; input >> sample; cnt++) {
        if (start_sample <= cnt and cnt <= end_sample) {
            all_samples.push_back(sample);
            for (const int i : others) {
                IntSample other;
                if (not input_ctx.GetInputStreamByIndex(i) >> other) {
                    break;
                }
                all_samples.push_back(other);
            }
            sample = mix_samples(all_samples);
            all_samples.clear();
        }
        output << sample;
    }

}


std::unique_ptr<IConverter> MixCreator::Create(Params params) const {

    if (params.streams.empty()) {
        throw std::runtime_error("Need at least 1 stream for mix");
    }

    std::vector<int> vars = get_streams_indexes(params.streams);

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

HelpDescriptor MixCreator::GetHelpDescriptor() const {
    return HelpDescriptor{
        "mix source into stream from start to end",
        {"source", "start", "end"},
        {"mix $2 50", "mix $1"}
    };
}

std::string MixCreator::GetName() const { return "mix";}
