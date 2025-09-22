//
// Created by kapiuser on 15.09.25.
//

#ifndef WORDCOUNTER_H
#define WORDCOUNTER_H


#include <map>
#include <string>
#include <boost/algorithm/string.hpp>

class WordCounter {
private:
    std::map<std::string, int> freqMap {};
    std::vector<std::pair<std::string, int>> freqVector {};
    int totalWords {};

    void CountWordsAndMap_(const std::string &text);


public:
    WordCounter() = default;
    ~WordCounter() = default;
    void CountWordsIn(const std::string &text);
    const std::vector<std::pair<std::string, int>>  &GetSortedVector();
    int GetTotal() const;

};



#endif //WORDCOUNTER_H
