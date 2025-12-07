#ifndef SPEED_UP_H
#define SPEED_UP_H


#include "../i_converter.h"
#include "../../converters-factory/converter_factory.h"

namespace converter {

    class Speed : public IConverter {

    public:
        explicit Speed(Seconds start);

        explicit Speed(Seconds start, Seconds end);

        const std::string GetName() override {return "speed";}

        void Apply(audio_stream::Context & input_ctx, IAudioOut & output) override;

    private:
        static constexpr int Unset = std::numeric_limits<Seconds>::max();

        int start_seconds = Unset;
        int end_seconds = Unset;
    };

    class SpeedCreator final : public IConverterCreator {
    public:
        std::unique_ptr<IConverter> Create(Params params) const override;

        HelpDescriptor GetHelpDescriptor() const override;

        std::string GetName() const override;

    private:
    };
}



#endif //SPEED_UP_H
