#ifndef MUTE_H
#define MUTE_H
#include <iostream>

#include "../i_converter.h"


namespace converter {
    class Mute : public IConverter {

    public:
        explicit Mute(Seconds start);
        explicit Mute(Seconds start, Seconds end);
        void Apply(IAudioIn &input, IAudioOut & output) override;

        std::string GetName() const override {return "mute";}

    };
}




#endif //MUTE_H
