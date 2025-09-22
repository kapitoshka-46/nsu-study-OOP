#include <fstream>
#include <iostream>
#include <vector>

#include "WordCounter.h"
#include "Writer.h"
#include "Reader.h"


int main(int argc, char** argv) {
    if (argc >= 3) {
        std::cout << "Count words: "<< argv[1] << " ==> " << argv[2] << std::endl;
    }
    else {
        std::cout << "Invalid argument. Usage: "<< argv[0] << " input.txt output.csv" <<'\n';
        return 0;
    }

    auto counter = WordCounter(argv[1]);
    const auto &freqVector = counter.GetStatistic();

    auto out = Writer(argv[2]);
    out.SaveAsCSV(freqVector, counter.TotalWordsCount());

    return 0;
}