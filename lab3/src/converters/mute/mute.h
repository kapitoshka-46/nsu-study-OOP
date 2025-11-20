#ifndef MUTE_H
#define MUTE_H
#include <iostream>

#include "../i_converter.h"


namespace converter {
    class Mute : public IConverter {

    public:
        explicit Mute(Seconds start);
        explicit Mute(Seconds start, Seconds end);
        void Apply(std::vector<int16_t> &samples) override;
    };
}




#endif //MUTE_H
