#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "../core/Core.h"
#include <iostream>


namespace file_handler {
    void LoadFromFile(std::ifstream &in, core::universe &u);
    void SaveToFile(std::ofstream &out, const core::universe &u);
}

#endif //FILE_HANDLER_H
