#include "mute.h"

using audio_stream::IntSample;

converter::Mute::Mute(Seconds start) : start_seconds(start) {
    if (start < Seconds(0)) {
        std::ostringstream iss;
        iss << "Invalid start timestamp: " << start;
        throw std::invalid_argument(iss.str());
    }
    std::cout << "const: mute from " << start << " to end" << "\n";
}

converter::Mute::Mute(Seconds start, Seconds end) : start_seconds(start), end_seconds(end) {
    if (start < Seconds{0}) {
        std::ostringstream iss;
        iss << "Invalid start timestamp: " << start;
        throw std::invalid_argument(iss.str());
    }


    if (end < Seconds{0}) {
        std::ostringstream iss;
        iss << "Invalid end timestamp: " << start;
        throw std::invalid_argument(iss.str());
    }

    if (end < start) {
        throw std::invalid_argument("end < start");
    }

    std::cout << "const: mute from " << start << " to " << end << "\n";
}

void converter::Mute::Apply(audio_stream::Context & input_ctx, IAudioOut & output) {
    IAudioIn &input = input_ctx.GetMainInputStream();
    const uint32_t start_sample = start_seconds * input.GetSampleRate();
    uint32_t end_sample = end_seconds * input.GetSampleRate();

    if (end_seconds == Unset) {
        end_sample = std::numeric_limits<uint32_t>::max();
    }

    IntSample sample;
    for (int cnt_sample = 0; input >> sample; cnt_sample++) {
        if (start_sample <= cnt_sample and cnt_sample <= end_sample) {
            sample = 0;
        }
        output << sample;
    }

}

converter::HelpDescriptor converter::Mute::GetHelpDescriptor() const {
    return HelpDescriptor{"Mutes from start to end in seconds",
        {"start", "end"},
        {"mute 10 20", "mute 5"}
    };
}
