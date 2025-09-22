#include "Writer.h"
#include <iostream>
#include <iomanip>

Writer::Writer(const char* outPath) {
    std::cout << "[Writer] File " << outPath << " opened." << '\n';
    out.open(outPath);
}
Writer::~Writer() {
    if (out.is_open()) {
        out.close();
    }
    std::cout << "[Writer] File closed." << '\n';
}

void Writer::SaveAsCSV(const std::vector<std::pair<const std::string *, int>> &vectorOfPairs, const int total) {

    for (const auto& [first, second]: vectorOfPairs) {
        double freqPercent = static_cast<double>(second) / total * 100;
        out << std::fixed << std::setprecision(0) << *first << ',' << second << ',' << freqPercent << '%' << '\n';
    }
    std::cout << "[Writer] Saved as CSV." << "\n";
}

