#ifndef WAV_STREAM_H
#define WAV_STREAM_H

#include <cstdint>
#include <fstream>
#include <string>

#include "../ISampleStreamInput.h"

namespace sample_stream {
    struct BaseChunk {
        union {
            int8_t chunk_id[4];     // careful. it is not a null-terminated C string!
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
    };

    class WAVStream : public ISampleStreamInput {
    protected:
        std::ifstream file;
        WavHeader header{};
    };

    class WAVStreamInput : public WAVStream {
    public:
        explicit WAVStreamInput(const std::string& filename);
        ~WAVStreamInput() override;

        explicit operator bool() const override {
            return static_cast<bool>(file);
        }

         ISampleStreamInput &operator>>(uint16_t &value) override {
            file.read(reinterpret_cast<char*>(&value), sizeof(value));
            return *this;
        }

    private:
        // inherited from WAVStream
        std::ifstream file;
        WavHeader header{};
    };
}




#endif //WAV_STREAM_H
