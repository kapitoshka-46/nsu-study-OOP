#include "DocumentWriter.h"
#include <iostream>

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

void DocumentWriter::saveAsCSV(std::vector<std::pair<std::string, int>> &vectorOfPairs) {
    for (const auto& [first, second]: vectorOfPairs) {
        out << first << ',' << second << '\n';
    }
    std::cout << "[Document Writer] Saved as CSV." << "\n";
}

#include "DocumentWriter.h"
