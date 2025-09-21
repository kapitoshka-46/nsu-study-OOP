#include "BitArray.h"

#include <iostream>

using uchar = unsigned char;


//---------------------------- Constructors -------------------------------//
BitArray::BitArray(int num_bits, unsigned long value)
: size_bits(num_bits) {

    if (num_bits < 0) {
        std::cerr << "Cannot create array with negative size\n";
        return;
    }
    bytes.resize(size_bytes_()); // size_bits should be initialized before this call
    for (int i = 0; i < bytes.size(); i++) {
        bytes[i] = static_cast<uchar>(value >> (i * 8) & 255);
    }
}

// ----------------------------- static methods ------------------------------ //



// ------------------------------ const methods ----------------------------- //
int BitArray::size() const {
    return size_bits;
}

bool BitArray::empty() const {
    return size_bits == 0;
}

int BitArray::size_bytes_() {
    int num_bytes = size_bits / 8;
    if (size_bits % 8 != 0) {
        num_bytes += 1;
    }
    return num_bytes;
}

std::string BitArray::to_string() const {
    std::string str;
    str.resize(size_bits);
    std::cerr << __FUNCTION__ << ": " <<  "str.size() = " << str.size() << std::endl;
    int size = static_cast<int>(bytes.size());
    for (int i = 0; i < size; i++) {
        for (int bit_index = 0;  bit_index < 8; bit_index++) {
            uchar bit = bytes[size - 1 - i] >> (7 - bit_index) & 1;
            str[i * 8 + bit_index] = bit == 1 ? '1' : '0';
        }
    }
    return str;
}

bool BitArray::any() const {
    return std::any_of(
        bytes.begin(),
        bytes.end(),
        [] (auto x) {return x != 0;});
}

bool BitArray::none() const {
    return std::none_of(
        bytes.begin(),
        bytes.end(),
        [] (auto x) {return x != 0;});
}

int BitArray::count() const {
    int count = 0;
    for (int i = 0; i < size_bits; i++) {
        count += this->operator[](i);
    }
    return count;
}

const std::vector<unsigned char> &BitArray::data() const {
    return bytes;
}

bool BitArray::is_equal(const BitArray &a, const BitArray &b) {
    return a.data() == b.data();
}

bool BitArray::is_not_equal(const BitArray &a, const BitArray &b) {
    return a.data() != b.data();
}


// ------------------------------- methods ------------------------------ //
void BitArray::swap(BitArray &b) {
    bytes.swap(b.bytes);
}

void BitArray::clear() {
    bytes.clear();
}

BitArray &BitArray::reset(int n) {
    int i = n / 8;
    auto mask = ~(1 << (n % 8));
    bytes[i] = bytes[i]  &  mask;
    return *this;
}
BitArray &BitArray::reset() {
    std::fill(bytes.begin(), bytes.end(),0);
    return *this;
}



BitArray &BitArray::set(int n, bool val) {
    int i = n / 8;
    reset(n);
    bytes[i] = bytes[i] | val << (n % 8);
    return *this;
}
BitArray &BitArray::set() {
    std::fill(bytes.begin(), bytes.end(), 1);
    return *this;
}


void BitArray::push_back(bool bit) {
    if (size_bits % 8 == 0) {
        bytes.push_back(static_cast<uchar>(bit));
    }
    else {
        set(size_bits, bit);
    }
    size_bits += 1;
}






// ---------------------------- operators ------------------------------ //
BitArray& BitArray::operator=(const BitArray &b) {
    if (&b != this) {
        bytes = b.bytes;
    }
    return *this;
}

bool BitArray::operator[](int i) const {
    return bytes[i / 8] >> i % 8;
}

BitArray BitArray::operator~() const {
    auto new_arr = BitArray(size_bits);
    for (auto& byte : new_arr.bytes) {
        byte = ~byte;
    }
    return new_arr;
}

bool operator==(const BitArray & a, const BitArray & b) {
    return BitArray::is_equal(a, b);
}

bool operator!=(const BitArray &a, const BitArray &b) {
    return a.data() != b.data();
}



