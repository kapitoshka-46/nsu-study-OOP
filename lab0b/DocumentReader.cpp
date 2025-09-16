#include "DocumentReader.h"

#include <iostream>

DocumentReader::DocumentReader(const char *path) {

    in.open(path);
    if (in.fail()) {
        std::cerr << "[Document Reader] Cannot open file " << path << '\n';
        exit(1);
    }
    std::cout << "[Document Reader] File "<< path << " opened." << '\n';

}

DocumentReader::~DocumentReader() {
    in.close();
    std::cout << "[Document Reader] File closed." << '\n';

};

std::string &DocumentReader::GetNewLine() {
    isEnd = not static_cast<bool>(std::getline(in, buffer));
    return buffer;
}

bool DocumentReader::IsEnd() {
    return isEnd;
}
