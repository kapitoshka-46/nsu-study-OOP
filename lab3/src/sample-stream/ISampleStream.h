#ifndef ISAMPLESTREAM_H
#define ISAMPLESTREAM_H
#include <cstdint>
#include <chrono>

namespace audio_stream {
    using IntSample = int16_t;
    using Seconds = int;

    inline int seconds_to_samples(int sec, int sample_rate) {return sec * sample_rate;}


    class BaseAudiostream {
    public:
        virtual ~BaseAudiostream() = default;

        virtual explicit operator bool() const = 0;

        virtual void Rewind() = 0;

        virtual void Skip(size_t num_of_samples) = 0;

        [[nodiscard]] virtual uint32_t GetSampleRate() const = 0;

        [[nodiscard]] virtual uint16_t GetDepth() const = 0;

        virtual void Seek(size_t num_of_samples, std::ios_base::seekdir dir) = 0;
    };

    // Interface for get samples from audio files.
    class IAudioIn : public BaseAudiostream{
    public:
        ~IAudioIn() override = default;

        virtual IAudioIn &operator>>(IntSample &value) = 0;

        explicit operator bool() const override = 0;

        void Rewind() override = 0;

        void Skip(size_t num_of_samples) override = 0;

        void Seek(size_t num_of_samples, std::ios_base::seekdir dir) override = 0;

    };

    // Interface for writing samples to audio files
    class IAudioOut : public BaseAudiostream{
    public:
        ~IAudioOut() override = default;

        virtual IAudioOut &operator<<(IntSample value) = 0;

        explicit operator bool() const override = 0;

        void Rewind() override = 0;

        void Skip(size_t num_of_samples) override = 0;

        void Seek(size_t num_of_samples, std::ios_base::seekdir dir) override = 0;

        [[nodiscard]] uint16_t GetDepth() const override = 0;

        [[nodiscard]] uint32_t GetSampleRate() const override = 0;

    };

    class Context{
    public:
        virtual ~Context() = default;
        virtual IAudioIn &GetMainInputStream() = 0;
        virtual void SetMainInputStream(const std::string &main_input_path) = 0;
        virtual IAudioIn &GetInputStreamByIndex(int index) = 0;
        virtual size_t GetNumOfStreams() = 0;
    };
}



#endif //ISAMPLESTREAM_H
