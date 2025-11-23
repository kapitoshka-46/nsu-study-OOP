#ifndef ISAMPLESTREAM_H
#define ISAMPLESTREAM_H
#include <cstdint>

namespace audio_stream {

    class BaseAudiostream {
    public:
        virtual ~BaseAudiostream() = default;

        virtual explicit operator bool() const = 0;

        virtual void Rewind() = 0;

        virtual void Skip(size_t num_of_samples) = 0;

        virtual void Seek(size_t num_of_samples, std::ios_base::seekdir dir) = 0;
    };

    // Interface for get samples from audio files.
    class IAudioIn : public BaseAudiostream{
    public:
        ~IAudioIn() override = default;

        virtual IAudioIn &operator>>(uint16_t &value) = 0;

        explicit operator bool() const override = 0;

        void Rewind() override = 0;

        void Skip(size_t num_of_samples) override = 0;

        void Seek(size_t num_of_samples, std::ios_base::seekdir dir) override = 0;

    };

    // Interface for writing samples to audio files
    class IAudioOut : public BaseAudiostream{
    public:
        ~IAudioOut() override = default;

        virtual IAudioOut &operator<<(uint16_t value) = 0;

        explicit operator bool() const override = 0;

        void Rewind() override = 0;

        void Skip(size_t num_of_samples) override = 0;

        void Seek(size_t num_of_samples, std::ios_base::seekdir dir) override = 0;

    };
}

#endif //ISAMPLESTREAM_H
