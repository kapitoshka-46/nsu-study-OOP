#include <fstream>
#include <iostream>
#include <vector>

#include "WordCounter.h"
#include "DocumentWriter.h"
#include "DocumentReader.h"

int main(int argc, char** argv) {
    using std::string;

    if (argc >= 3) {
        std::cout << "Count words: "<< argv[1] << " ==> " << argv[2] << std::endl;
    }
    else {
        std::cout << "Invalid argument. Usage: "<< argv[0] << " input.txt output.csv" <<'\n';
        return 0;
    }
    // TODO звездочки конст мейн упротсить
    auto in = DocumentReader(argv[1]);
    auto counter = WordCounter();
    string &line = in.GetNewLine();

    while (!in.IsEnd()) {
        counter.CountWordsIn(line);
        line = in.GetNewLine();
    }
    const auto &freqVector = counter.GetSortedVector();

    auto out = DocumentWriter(argv[2]);
    out.SaveAsCSV(freqVector, counter.GetTotal());
}