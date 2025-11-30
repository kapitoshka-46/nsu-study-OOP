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

        [[nodiscard]] std::string GetName() const override {return "mix";}

        [[nodiscard]] HelpDescriptor GetHelpDescriptor() const override;
    private:
        Seconds start_seconds = 0;
        Seconds end_seconds = Unset;
        std::vector<int>(others);

        static constexpr Seconds Unset = std::numeric_limits<Seconds>::max();

    };
}




#endif //MIN_H
