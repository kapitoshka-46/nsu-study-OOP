#ifndef MUTE_H
#define MUTE_H
#include <iostream>

#include "../i_converter.h"

namespace converter {

    class Mute : public IConverter {

    public:
        explicit Mute(Seconds start);
        explicit Mute(Seconds start, Seconds end);
        void Apply(audio_stream::Context & input_ctx, IAudioOut & output) override;

        [[nodiscard]] std::string GetName() const override {return "mute";}

        [[nodiscard]] HelpDescriptor GetHelpDescriptor() const override;

    private:
        static constexpr int Unset = std::numeric_limits<Seconds>::max();

        int start_seconds = Unset;
        int end_seconds = Unset;
    };
}




#endif //MUTE_H
