#include "WordCounter.h"
#include <algorithm>

void WordCounter::CountWordsAndMap_(const std::string &text) {
    // Find a word
    std::string separators = " ;:,./\'\"<>!?@()[]{}=+-\\|\n";
    size_t start = text.find_first_not_of(separators);
    while (start != std::string::npos) {
        size_t end = text.find_first_of(separators, start + 1);
        if (end == std::string::npos) { // == дошли до конца строки
            end = text.length() - 1;
        }
        // the word is founded
        auto word = text.substr(start, end - start);
        boost::algorithm::to_lower(word);

        totalWords++;
        freqMap[word] += 1;
        start = text.find_first_not_of(separators, end + 1);
    }
}


void WordCounter::CountWordsIn(const std::string &text) {
    CountWordsAndMap_(text);
}
// TODO поставить конст
const std::vector<std::pair<std::string, int>> &WordCounter::GetSortedVector() {
    using std::pair;
    using std::string;
    freqVector = {freqMap.begin(), freqMap.end()};
    std::sort(
        freqVector.begin(),
        freqVector.end(),
        [](const auto &a, const auto &b) {
            return a.second > b.second;
        }
        );
    return freqVector;
}

int WordCounter::GetTotal() const {
    return totalWords;
}
