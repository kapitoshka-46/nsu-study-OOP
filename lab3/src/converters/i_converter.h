#ifndef I_CONVERTER_H
#define I_CONVERTER_H
#include <chrono>
#include <vector>
#include "../sample-stream/ISampleStream.h"


namespace converter {
    // Audio Interfaces
    using audio_stream::IAudioIn;
    using audio_stream::IAudioOut;
    using audio_stream::Seconds;

    // TODO: убрать эти юзинги
    using Vars = std::vector<std::string>;  // $0 $1, ...
    using TimeStamps = std::vector<Seconds>;   // 20, 10, 0, ...

    struct Params {
        Vars streams;
        TimeStamps time_stamps;
    };

    struct HelpDescriptor {
        std::string description;
        std::vector<std::string> parameters;
        std::vector<std::string> examples;
    };

    class IConverter {
    public:
        virtual ~IConverter() = default;
        virtual void Apply(audio_stream::Context & input, IAudioOut &output) = 0;

        [[nodiscard]] virtual std::string GetName() const = 0;

        [[nodiscard]] virtual HelpDescriptor GetHelpDescriptor() const = 0;

    };

}



#endif //I_CONVERTER_H
