#ifndef MIN_H
#define MIN_H


#include "../i_converter.h"


namespace converter {
    class Mix : public IConverter{
    public:
        explicit Mix(std::vector<std::string> others);
        Mix(std::vector<std::string> others, Seconds start);
        Mix(std::vector<std::string> others, Seconds start, Seconds end);

        void Apply(IAudioIn &input, IAudioOut &output) override;

        std::string GetName() const override {return "mix";}
    };
}




#endif //MIN_H
