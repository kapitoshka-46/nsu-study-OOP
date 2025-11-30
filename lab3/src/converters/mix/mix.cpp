#include "mix.h"
#include "../../terminal/color.h"

#include <iostream>
#include <limits>

using audio_stream::IntSample;

void print_input_sources(std::vector<int> &vec) {
    std::cout << color::bold << color::bright_magenta << "files for mixing:\n" << color::reset;
    std::cout << color::magenta;
    for (auto x: vec) {
        std::cout << "\t" << x << "\n";
    }
    std::cout << color::reset;
    std::cout << "\n";
}

converter::Mix::Mix(std::vector<int> others) : others(others) {
    print_input_sources(others);
    std::cout << "const: mix for others\n";
}

converter::Mix::Mix(std::vector<int> others, Seconds start) : start_seconds(start), others(others) {
    if (start < 0) {throw std::invalid_argument("mix: start cannot be < 0");}
    print_input_sources(others);
    std::cout << "const: mix for others from " << start << " to end\n";
}

converter::Mix::Mix(std::vector<int> others, Seconds start, Seconds end)
: others(others),
start_seconds(start),
end_seconds(end) {
    if (start < 0) {throw std::invalid_argument("mix: start cannot be < 0");}
    if (end < start) {throw std::invalid_argument("mix: end cannot be < start");}
    print_input_sources(others);
    std::cout << "const: mix for others from " << start << " to " << end << "\n";
}


IntSample mix_samples(const std::vector<IntSample> &samples) {
    IntSample result = 0;
    int n = samples.size();
    for (IntSample sample : samples) {
        result += static_cast<IntSample>(sample / n);
    }
    return result;
}

void converter::Mix::Apply(audio_stream::Context & input_ctx, IAudioOut &output) {

    IAudioIn &input = input_ctx.GetMainInputStream();

    const uint32_t start_sample = start_seconds * input.GetSampleRate();
    uint32_t end_sample = end_seconds * input.GetSampleRate();
    if (end_seconds == Unset) {
        end_sample = Unset;
    }

    std::cout << "apply: mix \n";
    IntSample sample;
    std::vector<IntSample> all_samples;
    for (auto cnt = start_sample; input >> sample; cnt++) {
        if (start_sample <= cnt and cnt <= end_sample) {
            all_samples.push_back(sample);
            for (const int i : others) {
                IntSample other;
                input_ctx.GetInputStreamByIndex(i) >> other;
                all_samples.push_back(other);
            }
            sample = mix_samples(all_samples);
            all_samples.clear();
        }
        output << sample;
    }

}

converter::HelpDescriptor converter::Mix::GetHelpDescriptor() const {
    return HelpDescriptor{"Mix source into stream from start",
    {"source", "start=0"},
    {"mix $2 50", "mix $1"}
    };
}
