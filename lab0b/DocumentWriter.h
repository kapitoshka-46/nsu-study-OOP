//
// Created by kapiuser on 14.09.25.
//

#ifndef SAVER_H
#define SAVER_H

#include <string>
#include <fstream>
#include <vector>

class DocumentWriter {
private:
    int count {};
    std::ofstream out {};
public:
    DocumentWriter(const char  *outPath);
    ~DocumentWriter();
    void saveAsCSV(std::vector<std::pair<std::string, int>> &vectorOfPairs);
};



#endif //SAVER_H
