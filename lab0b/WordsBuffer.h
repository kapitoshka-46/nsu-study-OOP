#ifndef WORDS_BUFFER_H
#define WORDS_BUFFER_H

#include <array>
#include <string>
#include <fstream>


namespace words_buffer {
    constexpr size_t maxBufferSize = 512;

    class WordsBuffer {
        std::ifstream &in;
        std::array<std::string, maxBufferSize> words;
        
    public:
        WordsBuffer(std::ifstream &in);
        ~WordsBuffer() = default;

        const std::string &nextWord();
        bool isEndOfFile(void);
        bool isEmpty(void);     //
        void update(void);
        bool updateIfPossible(void);
    };
}

#endif // WORDS_BUFFER_H