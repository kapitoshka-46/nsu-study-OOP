#include "mute.h"

using audio_stream::IntSample;

using namespace converter;

AutoRegisterConverter<Mute> r {"mute", std::make_shared<MuteCreator>()};

Mute::Mute(Seconds start) : start_seconds(start) {
    if (start < Seconds{0}) {
        std::ostringstream iss;
        iss << "Invalid start timestamp: " << start;
        throw std::invalid_argument(iss.str());
    }
}

Mute::Mute(Seconds start, Seconds end) : start_seconds(start), end_seconds(end) {
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

}

void Mute::Apply(audio_stream::Context & input_ctx, IAudioOut & output) {
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

std::unique_ptr<IConverter> MuteCreator::Create(Params params) const {
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
}

HelpDescriptor MuteCreator::GetHelpDescriptor() const {
    return HelpDescriptor{"mutes from start to end",
    {"start", "end"},
    {"mute 10 20", "mute 5"}
    };
}

std::string MuteCreator::GetName() const {
    return "mute";
}
