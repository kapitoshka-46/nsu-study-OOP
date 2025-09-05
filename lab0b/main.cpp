#include <iostream>
#include "WordsBuffer.h"

int main() {
    using std::string;
    using namespace words_buffer;
    auto wordsBuffer = WordsBuffer();

    string text = "It is My message, here is some words.";
    wordsBuffer.makeWordsFromStr(text);
    for (auto& s : wordsBuffer.words) {
        std:: cout << s << '\n';
    }
    return 0;
}