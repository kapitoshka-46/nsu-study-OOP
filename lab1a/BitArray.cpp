#include "BitArray.h"

#include <iostream>

size_t BitArray::get_size_in_bytes_(int num_bits) {
    int size_in_bytes = num_bits / 8;
    if (num_bits % 8 != 0) {
        size_in_bytes += 1;
    }
    return size_in_bytes;
}


BitArray::BitArray() = default;

BitArray::BitArray(int num_bits, unsigned long value) {
    if (num_bits < 0) {
        std::cerr << "Cannot create array with negative size\n";
        return;
    }

    bytes.resize(get_size_in_bytes_(num_bits));
    size_bits = num_bits;
    for (int i = 0; i < bytes.size(); i++) {
        bytes[i] = (value >> (i * 8)) & 255;
    }
}

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


