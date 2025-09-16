#include <iostream>
#include "BitArray.h"

int main() {
    auto bitarr = BitArray(8, 10);
    std::cout << bitarr.to_string() << std::endl;
    return 0;
}