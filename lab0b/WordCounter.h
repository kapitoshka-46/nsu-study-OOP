//
// Created by kapiuser on 15.09.25.
//

#ifndef WORDCOUNTER_H
#define WORDCOUNTER_H


#include <map>
#include <string>
#include <boost/algorithm/string.hpp>
#include "Reader.h"

class WordCounter {
private:

    Reader input;
    std::map<std::string, int> freqMap {};
    std::vector<std::pair<const std::string *, int>> freqVector{};
    int totalWords {};

    void CountWordsAndMap_(const std::string &text);
    const std::vector<std::pair<const std::string *, int>> &GetSortedVector_();


public:
    WordCounter(const std::string& inputPath);
    ~WordCounter() = default;

    int TotalWordsCount() const;
    const std::vector<std::pair<const std::string *, int>> &GetStatistic();

};



#endif //WORDCOUNTER_H
