//
// Created by kapiuser on 15.09.25.
//

#ifndef DOCUMENTREADER_H
#define DOCUMENTREADER_H

#include <fstream>
#include <string>

class DocumentReader {
private:
    int count = 0;
    bool isEnd = false;
    std::ifstream in;
    std::string buffer;

public:

    DocumentReader(const char *path);
    ~DocumentReader();

    std::string &GetNewLine();
    bool IsEnd() const;
};


#endif //DOCUMENTREADER_H
