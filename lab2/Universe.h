//
// Created by kapiuser on 21.10.25.
//

#ifndef UNIVERSE_H
#define UNIVERSE_H
#include <vector>

#include "Display.h"
#include "UnixConsole.h"

class Universe {
private:
    static constexpr int MaxRow {100};
    static constexpr int MaxColumn {100};
    std::vector<std::vector<bool>> data_;

    Display display_;
    UnixConsole console_;
public:
    Universe();
};



#endif //UNIVERSE_H
