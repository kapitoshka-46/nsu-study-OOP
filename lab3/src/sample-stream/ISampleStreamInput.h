#ifndef ISAMPLESTREAM_H
#define ISAMPLESTREAM_H
#include <cstdint>

namespace sample_stream {

    // Interface for get samples from audio files.
    class ISampleStreamInput {
    public:
        virtual ~ISampleStreamInput() = default;

        virtual ISampleStreamInput &operator>>(uint16_t &value) = 0;

        virtual explicit operator bool() const = 0;
    };

    // Interface for writing samples to audio files
    class ISampleStreamOutput {
        virtual ISampleStreamOutput &StreamWrite(uint16_t value) = 0;
    public:
        virtual ~ISampleStreamOutput() = default;

        virtual ISampleStreamOutput &operator<<(uint16_t value) = 0;

        virtual explicit operator bool() const = 0;

    };
}

#endif //ISAMPLESTREAM_H
