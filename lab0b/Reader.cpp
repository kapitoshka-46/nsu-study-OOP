#include "Reader.h"

#include <iostream>

Reader::Reader(const std::string &path) {

    in.open(path);
    if (in.fail()) {
        std::cerr << "[Reader] Cannot open file " << path << '\n';
        exit(1);
    }
    std::cout << "[Reader] File "<< path << " opened." << '\n';

}

Reader::~Reader() {
    in.close();
    std::cout << "[Reader] File closed." << '\n';

};

std::string &Reader::GetNewLine() {
    isEnd = not static_cast<bool>(std::getline(in, buffer));
    return buffer;
}

bool Reader::IsEnd() const {
    return isEnd;
}
