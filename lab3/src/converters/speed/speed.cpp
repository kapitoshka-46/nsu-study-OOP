#include "speed.h"
#include <iostream>

using audio_stream::IntSample;
using namespace converter;

static AutoRegisterConverter<Speed> r {"speed", std::make_shared<SpeedCreator>()};

Speed::Speed(Seconds start) : start_seconds(start) {
    if (start < Seconds(0)) {
        std::ostringstream iss;
        iss << "Invalid start timestamp: " << start;
        throw std::invalid_argument(iss.str());
    }
}

Speed::Speed(Seconds start, Seconds end) : start_seconds(start), end_seconds(end) {
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

void Speed::Apply(audio_stream::Context & input_ctx, IAudioOut & output) {
    IAudioIn &input = input_ctx.GetMainInputStream();
    const uint32_t start_sample = start_seconds * input.GetSampleRate();
    uint32_t end_sample = end_seconds * input.GetSampleRate();

    if (end_seconds == Unset) {
        end_sample = std::numeric_limits<uint32_t>::max();
    }

    IntSample sample;
    IntSample another;
    for (int cnt_sample = 0; input >> sample; cnt_sample++) {
        output << sample;
        if (start_sample <= cnt_sample and cnt_sample <= end_sample) {
                input >> another; //no effect
        }
    }
}


std::unique_ptr<IConverter> SpeedCreator::Create(Params params) const {
    if (not params.streams.empty()) {
        throw std::invalid_argument("Speedup converter does not use streams");
    }

    auto size = params.time_stamps.size();
    if (size == 1) {
        auto start = params.time_stamps[0];
        return std::make_unique<Speed>(start);
    }
    else if (size == 2) {
        auto start = params.time_stamps[0];
        auto end = params.time_stamps[1];

        return std::make_unique<Speed>(start, end);
    }
    else  {
        throw std::runtime_error("Invalid timestamp size: " + std::to_string(size));
    }
}

HelpDescriptor SpeedCreator::GetHelpDescriptor() const {
    return HelpDescriptor{"speed up (same as x2)",
        {"start", "end"},
        {"speed 10 20", "speed 5"}
    };
}

std::string SpeedCreator::GetName() const {
    return "speed";
}
