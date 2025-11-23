#ifndef I_CONVERTER_H
#define I_CONVERTER_H
#include <chrono>
#include <vector>
#include "../sample-stream/ISampleStream.h"


namespace converter {
    // Audio Interfaces
    using audio_stream::IAudioIn;
    using audio_stream::IAudioOut;

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
        virtual void Apply(IAudioIn &input, IAudioOut &output) = 0;

        virtual std::string GetName() const = 0;
    };

}



#endif //I_CONVERTER_H
