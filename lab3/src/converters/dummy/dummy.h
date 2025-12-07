#ifndef DUMMY_H
#define DUMMY_H
#include "../i_converter.h"


namespace converter {

    // Do nothing. Just skip samples
    class Dummy : public IConverter {
    public:
        Dummy() = default;
        void Apply(audio_stream::Context & input_ctx, IAudioOut &output) override;

        const std::string GetName() override {return "dummy";}

    };

    class DummyCreator final : public IConverterCreator {
    public:
        std::unique_ptr<IConverter> Create(Params params) const override;

        HelpDescriptor GetHelpDescriptor() const override;

        std::string GetName() const override;

    private:
    };
}



#endif //DUMMY_H
