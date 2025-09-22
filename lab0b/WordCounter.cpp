#include "WordCounter.h"
#include <algorithm>


#include <iostream>


WordCounter::WordCounter(const std::string &inputPath)
    : input(inputPath)
{

}


void WordCounter::CountWordsAndMap_(const std::string &text) {
    // Find a word
    std::string separators = " ;:,./\'\"<>!?@()[]{}=+-\\|\n";
    size_t start = text.find_first_not_of(separators);
    while (start != std::string::npos) {
        size_t end = text.find_first_of(separators, start + 1);
        if (end == std::string::npos) { // == дошли до конца строки
            end = text.length();
        }
        // the word is founded
        auto word = text.substr(start, end - start);
        boost::algorithm::to_lower(word);

        totalWords++;
        freqMap[word] += 1;
        start = text.find_first_not_of(separators, end + 1);
    }
}

const std::vector<std::pair<const std::string *, int> > &WordCounter::GetStatistic() {
    auto line = input.GetNewLine();
    while (not input.IsEnd()) {
        line = input.GetNewLine();
        CountWordsAndMap_(line);
    }
    return GetSortedVector_();
}


const std::vector<std::pair<const std::string *, int>> &WordCounter::GetSortedVector_() {

    for (const auto &[fst, snd] : freqMap) {
        freqVector.emplace_back(&fst, snd);
    }

    std::sort(
        freqVector.begin(),
        freqVector.end(),
        [](const auto &a, const auto &b) {
            return a.second > b.second;
        });
    return freqVector;
}

int WordCounter::TotalWordsCount() const {
    return totalWords;
}
