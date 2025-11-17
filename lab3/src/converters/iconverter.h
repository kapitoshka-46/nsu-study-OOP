#ifndef ICONVERTER_H
#define ICONVERTER_H
#include <chrono>
#include <string>
#include <vector>

namespace converter {
    using Seconds = std::chrono::seconds;
    struct ConverterParams {
        std::vector<Seconds> time_stamps;
        std::vector<int> sources = {};
        ConverterParams(std::vector<Seconds> time_stamps, std::vector<int> sources = {});

    };

    class IConverter
    {
    public:
        virtual ~IConverter() = default;

        virtual void Apply() = 0;
    };
}
#endif // ICONVERTER_H
