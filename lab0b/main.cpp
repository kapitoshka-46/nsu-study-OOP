#include <iostream>

#include "WordCounter.h"
#include "Writer.h"


int main(int argc, char** argv) {
    if (argc >= 3) {
        std::cout << "Count words: "<< argv[1] << " ==> " << argv[2] << std::endl;
    }
    else {
        std::cout << "Invalid argument. Usage: "<< argv[0] << " input.txt output.csv" <<'\n';
        return 0;
    }
    const auto in_path = argv[1];
    const auto out_path = argv[2];

    auto counter = WordCounter(in_path);
    const auto &freqTable = counter.GetStatistic();

    auto out = Writer(out_path);
    out.SaveAsCSV(freqTable, counter.TotalWordsCount());

    return 0;
}