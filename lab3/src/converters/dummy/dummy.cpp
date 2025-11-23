#include "dummy.h"

void converter::Dummy::Apply(IAudioIn &input, IAudioOut &output) {
    uint16_t sample;
    while (input >> sample) {
        output << sample;
    }
}
