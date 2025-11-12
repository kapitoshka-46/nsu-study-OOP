#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "Core.h"
#include <iostream>


namespace file_handler {
    void LoadFromFile(std::ifstream &in, core::Universe& u);
}

#endif //FILE_HANDLER_H
