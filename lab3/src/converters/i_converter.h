#ifndef I_CONVERTER_H
#define I_CONVERTER_H
#include <chrono>
#include <vector>

namespace converter {
    using Seconds = std::chrono::seconds;
    using Vars = std::vector<std::string>;  // $0 $1, ...
    using TimeStamps = std::vector<Seconds>;   // 20, 10, 0, ...
    struct Params {
        Vars streams;
        TimeStamps time_stamps;
    };

    class IConverter {
    public:
        virtual ~IConverter() = default;
        virtual void Apply(std::vector<int16_t> &samples) = 0;
    };

}



#endif //I_CONVERTER_H
