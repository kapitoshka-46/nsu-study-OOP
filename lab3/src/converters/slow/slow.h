#ifndef SLOW_H
#define SLOW_H

#include "../i_converter.h"
#include "../../converters-factory/converter_factory.h"

namespace converter {

    class Slow : public IConverter {

    public:
        explicit Slow(Seconds start);

        explicit Slow(Seconds start, Seconds end);

        void Apply(audio_stream::Context & input_ctx, IAudioOut & output) override;

        const std::string GetName() override {return "slow";}

    private:
        static constexpr int Unset = std::numeric_limits<Seconds>::max();

        int start_seconds = Unset;
        int end_seconds = Unset;
    };

    class SlowCreator final : public IConverterCreator {
    public:
        std::unique_ptr<IConverter> Create(Params params) const override;

        HelpDescriptor GetHelpDescriptor() const override;

        std::string GetName() const override;

    private:
    };
}




#endif //SLOW_H
