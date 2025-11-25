#include "dummy.h"

void converter::Dummy::Apply(IAudioIn &input, IAudioOut &output) {
    uint16_t sample;
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
