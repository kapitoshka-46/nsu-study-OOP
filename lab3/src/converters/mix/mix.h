#ifndef MIN_H
#define MIN_H


#include "../i_converter.h"


namespace converter {
    class Mix : public IConverter{
    public:
        explicit Mix(std::vector<std::string> others);
        Mix(std::vector<std::string> others, Seconds start);
        Mix(std::vector<std::string> others, Seconds start, Seconds end);

        void Apply(std::vector<int16_t> &samples) override;
    };
}




#endif //MIN_H
