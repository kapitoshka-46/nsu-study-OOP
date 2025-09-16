#include "WordCounter.h"
#include <algorithm>

void WordCounter::CountWordsAndMap_(const std::string &text) {
    // Find a word
    std::string separators = " ;:,./\'\"<>!?@()[]{}=+-\\|\n";
    size_t start = text.find_first_not_of(separators);
    while (start != std::string::npos) {
        size_t end = text.find_first_of(separators, start + 1);
        if (end == std::string::npos) {
            end = text.length() - 1;
        }
        // the word is founded
        auto word = text.substr(start, end - start);
        boost::algorithm::to_lower(word);
        freqMap[word] += 1;
        start = text.find_first_not_of(separators, end + 1);
    }
}


void WordCounter::CountWordsIn(const std::string &text) {
    CountWordsAndMap_(text);
}

bool comapre(const std::pair<std::string, int> & a, const std::pair<const std::string, int>& b) {
    return a.second > b.second;
}

std::vector<std::pair<std::string, int>> & WordCounter::GetSortedVector() {
    // TODO вынести этот бардак в функцию.
    using std::pair;
    using std::string;
    freqVector = {freqMap.begin(), freqMap.end()};
    std::sort(
        freqVector.begin(),
        freqVector.end(),
        comapre
        );
    return freqVector;
}
