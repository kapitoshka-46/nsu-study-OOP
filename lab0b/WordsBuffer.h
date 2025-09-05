#ifndef WORDS_BUFFER_H
#define WORDS_BUFFER_H

#include <array>
#include <string>
#include <fstream>
#include <vector>


namespace words_buffer {
    // задача. чтение из файла и возвращение слов.
    class WordsBuffer {
    public:
        std::string line;
        std::vector<std::string> words;


        WordsBuffer() = default;
        ~WordsBuffer() = default;

        void makeWordsFromStr(std::string& text); // make private

        std::vector<std::string>& getNewWords();
        const std::string &nextWord();

    };
}

#endif // WORDS_BUFFER_H