#ifndef WAV_STREAM_H
#define WAV_STREAM_H

#include <cstdint>
#include <cstring>
#include <fstream>
#include <string>

#include "../ISampleStream.h"

namespace audio_stream {
    struct BaseChunk {
        union {
            int8_t chunk_id[4];     // careful. it is not a null-terminated C string! -- do not do `reinterpret_cast<char*>(&chunk_id) !!!!!!!`
            int32_t chunk_id_i;
        };
        uint32_t chunk_size;

        BaseChunk &Read(std::ifstream &file);
        [[nodiscard]] std::string GetID_Str() const;
    };

    struct RIFFChunk : BaseChunk {
        union {
            int8_t format[4];
            int32_t format_i;
        };

        RIFFChunk &Read(std::ifstream &file);

        [[nodiscard]] std::string GetFormatStr() const;

        bool IsValid() const;
    };

    struct FMTChunk : BaseChunk {
        union {
            int8_t audio_format[2]; // careful. it is not null-terminated C string!
            int16_t audio_format_i;
        };
        uint16_t num_channels;
        uint32_t sample_rate;
        uint32_t byte_rate;
        uint16_t block_align;
        uint16_t bits_per_sample;

        FMTChunk &Read(std::ifstream &file);
    };


    struct WavHeader {
        RIFFChunk riff_chunk;
        FMTChunk fmt_chunk;
        // ignore other chunks until "data" sub-chunk
        BaseChunk data_header; // only chunkID and size. data is not a part of header!

        void Write(std::ofstream & file);
    };

    class WAVStreamInput : public IAudioIn {
    public:
        explicit WAVStreamInput(const std::string& input_filename);
        ~WAVStreamInput() override;

        explicit operator bool() const override {
            return static_cast<bool>(file);
        }

         IAudioIn &operator>>(uint16_t &value) override {
            file.read(reinterpret_cast<char*>(&value), sizeof(value));
            return *this;
        }

        void Skip(size_t num_of_samples) override;

        void Seek(size_t num_of_samples, std::ios_base::seekdir dir) override {
            file.seekg(num_of_samples * header.fmt_chunk.bits_per_sample, dir);
        };

        uint32_t GetSampleRate() const override {return header.fmt_chunk.sample_rate;}

        uint16_t GetDepth() const override {return header.fmt_chunk.bits_per_sample;}

        void Rewind() override;
    private:
        std::ifstream file;
        WavHeader header{};
    };

    class WAVStreamOutput : public IAudioOut {
    public:
        explicit WAVStreamOutput(const std::string &ouput_filename,
            uint32_t sample_rate,
            uint16_t depth
            /* other params for header*/);
        ~WAVStreamOutput() override;

        explicit operator bool() const override {
            return static_cast<bool>(file);
        }

        IAudioOut &operator<<(uint16_t value) override {
            file.write(reinterpret_cast<char*>(&value), sizeof(value));
            return *this;
        }

        void Seek(size_t num_of_samples, std::ios_base::seekdir dir) override {
            file.seekp(num_of_samples * depth / 8, dir);
        }

        void Skip(size_t num_of_samples) override {
            file.seekp(num_of_samples * depth / 8, std::ios::cur);
        }

        uint32_t GetSampleRate() const override {return sample_rate;}

        uint16_t GetDepth() const override {return depth;}

        void Rewind() override;
    private:
        std::ofstream file;
        uint32_t sample_rate;
        uint16_t depth;
    };
}




#endif //WAV_STREAM_H
