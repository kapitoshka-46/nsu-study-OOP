#include <iostream>
#include <bitset>
#include "BitArray.h"

int main() {
    constexpr int num_bits = 8;
    constexpr int value = 11;


    std::cout << "--- My realisation ---\n";
    auto bitarr = BitArray(num_bits, value);
    std::cout << bitarr.to_string() << std::endl;


    std::cout << "--- STL realisation ---\n";
    auto cpparr = std::bitset<num_bits>(value);
    std::cout << cpparr << std::endl;
    return 0;
}