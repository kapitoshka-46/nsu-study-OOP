#include "dummy.h"

void converter::Dummy::Apply(audio_stream::Context & input_ctx, IAudioOut &output) {
    auto &input  = input_ctx.GetMainInputStream();
    audio_stream::IntSample sample;
    if (!input) {throw std::invalid_argument("no input");}
    if (!output) {throw std::invalid_argument("no output");}
    while (input >> sample) {
        output << sample;
    }
}

converter::HelpDescriptor converter::Dummy::GetHelpDescriptor() const {
    return HelpDescriptor{"Do nothing",
    {},
    {"dummy"}
    };
}
