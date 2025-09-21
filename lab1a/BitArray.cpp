#include "BitArray.h"

#include <iostream>

using uchar = unsigned char;


//---------------------------- Constructors -------------------------------//
BitArray::BitArray() = default;

BitArray::BitArray(int num_bits, unsigned long value) {
    if (num_bits < 0) {
        std::cerr << "Cannot create array with negative size\n";
        return;
    }

    int num_bytes = num_bits / 8;
    if (num_bits % 8 == 0) {
        num_bytes += 1;
    }

    bytes.resize(num_bytes);
    size_bits = num_bits;
    for (int i = 0; i < bytes.size(); i++) {
        bytes[i] = static_cast<uchar>(value >> (i * 8) & 255);
    }
}

BitArray::BitArray(const BitArray &b) {

}




// ------------------------------ const methods ----------------------------- //
int BitArray::size() const {
    return size_bits;
}


std::string BitArray::to_string() const {

    std::string str;
    str.resize(size_bits);
    for (int i = 0; i < bytes.size(); i++) {
        for (int bit_index = 0;  bit_index < 8; bit_index++) {
            unsigned char bit = (bytes[i] >> (7 - bit_index)) & 1;
            str[i * 8 + bit_index] = bit == 1 ? '1' : '0';
        }
    }
    return str;
}


