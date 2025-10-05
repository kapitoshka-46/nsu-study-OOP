#include <iostream>
#include <bitset>
#include "BitArray.h"



int main() {
    constexpr int num_bits = 32;
    constexpr int value = 2022214242;
    constexpr int offset = 4;
    std::cout << "--- My realisation ---\n";


    BitArray b1 {num_bits, value};
    std::cout << b1 << std::endl;

    b1.resize(10);
    std::cout << b1 << std::endl;

    b1.resize(32, false);
    std::cout << b1 << std::endl;

// ------------------------------------------------------------------------
    //
    // std::cout << "--- STL realisation ---\n";
    //
    //
    // auto c1 = std::bitset<num_bits>(value);
    // auto c2 = std::bitset<num_bits>(value - offset);
    // std::cout << c1 << '\n';
    // std::cout << c2 << '\n';
    // std::cout << (c1 & c2).to_string() << '\n';

    return 0;
}
