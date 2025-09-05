#include "WordsBuffer.h"

using namespace words_buffer;
void WordsBuffer::makeWordsFromStr(std::string& text) {
    using std::string;
    const string separators = " ,.:;/\"\'[]()\n?!";
    size_t start = text.find_first_not_of(separators);
    while (start != string::npos) {
        size_t end = text.find_first_of(separators, start + 1);
        if (end == string::npos) {
            end = text.length();
        }
        words.push_back(text.substr(start, end - start));
        start = text.find_first_not_of(separators, end + 1);
    }
}

