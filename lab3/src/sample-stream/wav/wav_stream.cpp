#include "wav_stream.h"

#include <iostream>

// всегда читаем BaseChunk
// если это интересующий нас чанк -- передаём
// считанный BaseChunk в конструктор другому чанку!

using namespace audio_stream;

BaseChunk &BaseChunk::Read(std::ifstream &file) {
    file.read(reinterpret_cast<char*>(this), sizeof(*this));
    return *this;
}

std::string BaseChunk::GetID_Str() const {
    std::string str;
    str.resize(4);
    str[0] = chunk_id[0];
    str[1] = chunk_id[1];
    str[2] = chunk_id[2];
    str[3] = chunk_id[3];
    return str;
}

RIFFChunk & RIFFChunk::Read(std::ifstream &file) {
    if (chunk_size == 0) {
        file.read(reinterpret_cast<char*>(this), sizeof(*this));
    }
    else {
        throw std::logic_error("read after read");  // TODO: fix this architecture issue
    }
    return *this;
}

std::string RIFFChunk::GetFormatStr() const {
    return std::string{reinterpret_cast<const char*>(format)};
}

bool RIFFChunk::IsValid() const {
    return GetID_Str() == "RIFF" and GetFormatStr() == "WAVE";

}

FMTChunk & FMTChunk::Read(std::ifstream &file) {
    file.read(reinterpret_cast<char*>(this), sizeof(*this));
    return *this;
}

void WavHeader::Write(std::ofstream &file) {
    file.write(reinterpret_cast<char*>(this), sizeof(*this));

}

// https://en.wikipedia.org/wiki/WAV
WAVStreamInput::WAVStreamInput(const std::string& input_filename) {
    // reading header until `data` subchunk


    file.open(input_filename, std::ios::binary);

    if (!file) {
        throw std::invalid_argument("Cannot open file: " + input_filename);
    }
    auto &riff = header.riff_chunk;
    auto &fmt = header.fmt_chunk;
    auto &data_header = header.data_header;

    // ------------------------------ RIFF Chunk --------------------------------------------
    riff.Read(file);
    if (!riff.IsValid()) {
        throw std::invalid_argument("Unsupported file format in file: " + input_filename);
    }

    bool is_found_data = false;
    while (file) {
        BaseChunk chunk{};
        chunk.Read(file);
        // ----------------------------- fmt chunk --------------------------------------
        if (chunk.GetID_Str() == "fmt ") {
           // std::cout << "found `format` subchunk" << std::endl;
            file.seekg(-static_cast<int>(sizeof(chunk)), std::ios_base::cur);
            fmt.Read(file);
            // std::cout << "\tsample rate: " << fmt.sample_rate << "\n";
            // std::cout << "\tdepth: " << fmt.bits_per_sample << "\n";
            // std::cout << "\tnum of channels: " << fmt.num_channels << "\n";
            // std::cout << "\taudio format: " << fmt.audio_format_i << "\n";
            // std::cout << "\tbytes for 1 sample and all channels : " << fmt.block_align << "\n";

            if (fmt.sample_rate != 44100) {
                throw std::invalid_argument("Unsupported sample rate: " + std::to_string(fmt.sample_rate));
            }
            if (fmt.bits_per_sample != 16) {
                throw std::invalid_argument("Unsupported bits per sample: "+ std::to_string(fmt.bits_per_sample));
            }
            if (fmt.num_channels != 1) {
                throw std::invalid_argument("Unsupported num of channels: " + std::to_string(fmt.num_channels));
            }
            if (fmt.audio_format_i != 1) {
                throw std::invalid_argument("Unsupported zipped audio");
            }
        }

        // -------------------------- data chunk --------------------------------
        else if (chunk.GetID_Str() == "data") {
        //    std::cout << "found `data` subchunk" << std::endl;
            is_found_data = true;
            data_header = chunk;
            break;
        }
        else {
            // skip any other chunk
            file.seekg(chunk.chunk_size, std::ios::cur);
        }
    }
    if (fmt.GetID_Str() != "fmt ") {
        throw std::invalid_argument("no fmt chunk founded");
    }
    if (!is_found_data) {
        throw std::invalid_argument("Cannot find audio. Corrupted file :\\");
    }
}

WAVStreamInput::~WAVStreamInput() {
    file.close();
}

void WAVStreamInput::Skip(size_t num_of_samples) {
    file.seekg(num_of_samples * (GetDepth() / 8), std::ios::cur);
}

void WAVStreamInput::Rewind() {
    file.seekg(sizeof(WavHeader), std::ios::beg);
}

WAVStreamOutput::WAVStreamOutput(const std::string &ouput_filename, uint32_t sample_rate, uint16_t depth)
: file(ouput_filename, std::ios::binary),
sample_rate(sample_rate),
depth(depth) {
    // a lot of specific file format code !!
    // works with mono wav
    if (depth % 8 != 0) { throw std::invalid_argument("depth is not divides by 8:" + std::to_string(depth)); }

    WavHeader header{};

    RIFFChunk &riff = header.riff_chunk;
    FMTChunk &fmt = header.fmt_chunk;
    BaseChunk &data = header.data_header;
    // ----------------- RIFF ------------------------
    std::memcpy(riff.chunk_id, "RIFF", 4);
    riff.chunk_size = sizeof(RIFFChunk) - sizeof(riff.chunk_id_i) - sizeof(riff.chunk_size);
    std::memcpy(riff.format, "WAVE", 4);

    // ----------------- FMT ---------------------------
    std::memcpy(fmt.chunk_id, "fmt ", 4);
    fmt.chunk_size = sizeof(FMTChunk) - sizeof(fmt.chunk_id_i) - sizeof(fmt.chunk_size);
    fmt.block_align = depth / 8;
    fmt.bits_per_sample = depth;
    fmt.sample_rate = sample_rate;
    fmt.num_channels = 1;
    fmt.byte_rate = sample_rate * depth * fmt.num_channels / 8;
    fmt.audio_format_i = 1;

    // ------------------ data -----------------s---------
    std::memcpy(header.data_header.chunk_id, "data", 4);
    data.chunk_size = static_cast<uint32_t>(-1);

    header.Write(file);
}

WAVStreamOutput::~WAVStreamOutput() {
    FlushBufferAndResetIter();
    file.close();
}

void WAVStreamOutput::Rewind() {
    file.seekp(sizeof(WavHeader), std::ios::beg);
}

