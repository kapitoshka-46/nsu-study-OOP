#include "wav_stream.h"

#include <iostream>

// всегда читаем BaseChunk
// если это интересующий нас чанк -- передаём
// считанный BaseChunk в конструктор другому чанку!


sample_stream::BaseChunk &sample_stream::BaseChunk::Read(std::ifstream &file) {
    file.read(reinterpret_cast<char*>(this), sizeof(*this));
    return *this;
}

std::string sample_stream::BaseChunk::GetID_Str() const {
    std::string str;
    str.resize(4);
    str[0] = chunk_id[0];
    str[1] = chunk_id[1];
    str[2] = chunk_id[2];
    str[3] = chunk_id[3];
    return str;
}

sample_stream::RIFFChunk & sample_stream::RIFFChunk::Read(std::ifstream &file) {
    if (chunk_size == 0) {
        file.read(reinterpret_cast<char*>(this), sizeof(*this));
    }
    else {
        throw std::logic_error("read after read");  // TODO: fix this architecture issue
    }
    return *this;
}

std::string sample_stream::RIFFChunk::GetFormatStr() const {
    return std::string{reinterpret_cast<const char*>(format)};
}

bool sample_stream::RIFFChunk::IsValid() const {
    return GetID_Str() == "RIFF" and GetFormatStr() == "WAVE";

}

sample_stream::FMTChunk & sample_stream::FMTChunk::Read(std::ifstream &file) {
    file.read(reinterpret_cast<char*>(this), sizeof(*this));
    return *this;
}

sample_stream::WAVStreamInput::WAVStreamInput(const std::string& filename) {
    // reading header until `data` subchunk.
    // https://en.wikipedia.org/wiki/WAV

    file.open(filename, std::ios::binary);

    if (!file) {
        throw std::invalid_argument("Cannot open file:" + filename);
    }
    auto &riff = header.riff_chunk;
    auto &fmt = header.fmt_chunk;
    auto &data_header = header.data_header;

    // ------------------------------ RIFF Chunk --------------------------------------------
    riff.Read(file);
    if (!riff.IsValid()) {
        throw std::invalid_argument("Unsupported file format in file: " + filename);
    }

    bool is_found_data = false;
    while (file) {
        BaseChunk chunk{};
        chunk.Read(file);
        // ----------------------------- fmt chunk --------------------------------------
        if (chunk.GetID_Str() == "fmt ") {
            std::cout << "found `format` subchunk" << std::endl;
            file.seekg(-static_cast<int>(sizeof(chunk)), std::ios_base::cur);
            fmt.Read(file);
            std::cout << "\tsample rate: " << fmt.sample_rate << "\n";
            std::cout << "\tdepth: " << fmt.bits_per_sample << "\n";
            std::cout << "\tnum of channels: " << fmt.num_channels << "\n";
            std::cout << "\taudio format: " << fmt.audio_format_i << "\n";
            std::cout << "\tbytes for 1 sample and all channels : " << fmt.block_align << "\n";

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
        else if (chunk.GetID_Str() == "data") {
            std::cout << "found `data` subchunk" << std::endl;
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

sample_stream::WAVStreamInput::~WAVStreamInput() {
    file.close();
}

