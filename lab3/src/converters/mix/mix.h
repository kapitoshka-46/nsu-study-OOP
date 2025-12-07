#ifndef MIN_H
#define MIN_H


#include "../i_converter.h"


namespace converter {
    class Mix : public IConverter{
    public:

        explicit Mix(std::vector<int> others);
        Mix(std::vector<int> others, Seconds start);
        Mix(std::vector<int> others, Seconds start, Seconds end);

        void Apply(audio_stream::Context & input_ctx, IAudioOut &output) override;

        const std::string GetName() override {return "mix";}
    private:
        Seconds start_seconds = 0;
        Seconds end_seconds = Unset;
        std::vector<int>(others);

        static constexpr Seconds Unset = std::numeric_limits<Seconds>::max();

    };

    class MixCreator final : public IConverterCreator {
    public:
        std::unique_ptr<IConverter> Create(Params params) const override;

        HelpDescriptor GetHelpDescriptor() const override;

        std::string GetName() const override;

    private:
    };
}




#endif //MIN_H
