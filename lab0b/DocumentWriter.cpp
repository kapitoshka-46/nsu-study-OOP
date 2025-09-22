#include "DocumentWriter.h"
#include <iostream>
#include <iomanip>

DocumentWriter::DocumentWriter(const char* outPath) {
    std::cout << "[Document Writer] File " << outPath << " opened." << '\n';
    out.open(outPath);
}
DocumentWriter::~DocumentWriter() {
    if (out.is_open()) {
        out.close();
    }
    std::cout << "[Document Writer] File closed." << '\n';
}

void DocumentWriter::SaveAsCSV(const std::vector<std::pair<const std::string *, int>> &vectorOfPairs, const int total) {

    for (const auto& [first, second]: vectorOfPairs) {
        double freqPercent = static_cast<double>(second) / total * 100;
        out << std::fixed << std::setprecision(2) << *first << ',' << second << ',' << freqPercent << '%' << '\n';
    }
    std::cout << "[Document Writer] Saved as CSV." << "\n";
}

