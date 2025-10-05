#include "BitArray.h"

#include <iostream>
#include <gtest/gtest.h>

using uchar = unsigned char;

//---------------------------- Constructors -------------------------------//
BitArray::BitArray(int num_bits, const unsigned long value)
: size_bits(num_bits) {
    if (num_bits < 0) {
        throw std::invalid_argument("Cannot create array with negative size");
    }
    bytes.resize(size_bytes_()); // size_bits should be initialized before this call

    if (value != 0) {
        for (int i = 0; i < bytes.size(); i++) {
            bytes[i] = static_cast<uchar>(value >> (i * 8) & 255);
        }
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

int BitArray::size_bytes_() const {
    int num_bytes = size_bits / 8;
    if (size_bits % 8 != 0) {
        num_bytes += 1;
    }
    return num_bytes;
}

std::string BitArray::to_string() const {
    std::string str;
    str.resize(size_bits);
    const BitArray &ref = *this;
    for (int i = 0; i < size_bits; i++) {
        str.at(size_bits - i - 1) = ref[i] == 0 ? '0' : '1';
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

BitArray BitArray::do_bit_operation(const BitArray &a, const BitArray &b, const operation op) {
    if (a.size_bits != b.size_bits) {
        throw std::invalid_argument("The arrays must be the shame size");
    }
    int sz = a.size_bits;
    BitArray result {};
    result.bytes.reserve(sz);
    result.size_bits = sz;

    auto op_func = [op](decltype(a.bytes[0]) elem1, decltype(b.bytes[0]) elem2) {
        switch(op) {
            case operation::AND  :  return elem1 & elem2;
            case operation::OR   :  return elem1 | elem2;
            case operation::XOR  :  return elem1 ^ elem2;
            default              :  throw std::invalid_argument("Unknown operation!");
        }
    };

    for (int i = 0; i < a.bytes.size(); ++i) {
        auto elem1 = a.bytes[i];
        auto elem2 = b.bytes[i];
        decltype(elem1) res {};
        res = op_func(elem1, elem2);
        result.bytes.emplace_back(res);
    }

    if (op == operation::XOR) {     // reset bits which has been `false` in the last byte before xor.
        for (int i = sz - sz % 8; i < sz; i++) {
            result.reset(i - 1);
        }
    }
    return result;
}

// ------------------------------- methods ------------------------------ //
void BitArray::swap(BitArray &b) {
    bytes.swap(b.bytes);
}

void BitArray::clear() {
    bytes.clear();
}

BitArray &BitArray::reset(int n) {
    if (n < 0) {
        throw std::out_of_range("Cannot reset negative bit");
    }
    int i = n / 8;
    auto mask = ~(1 << (n % 8));
    bytes[i] = bytes[i]  &  mask;
    return *this;
}
BitArray &BitArray::reset() {
    std::fill(bytes.begin(), bytes.end(),0);
    return *this;
}


BitArray BitArray::operator<<(int n) const {
    int offset_in_elem = n % 8;
    int global_offset = n / 8;
    BitArray new_arr {size_bits};

    if (global_offset > size_bits) {
        return new_arr;
    }

    new_arr.bytes.at(global_offset) = bytes.at(0) << offset_in_elem;
    for (int i = global_offset + 1; i < size_bytes_(); ++i) {
        new_arr.bytes.at(i) = (bytes.at(i - global_offset) << offset_in_elem) | (bytes.at(i - global_offset - 1) >> offset_in_elem);
    }

    return new_arr;
}


BitArray BitArray::operator>>(int n) const {
    int offset_in_elem = n % 8;
    int global_offset = n / 8;
    BitArray new_arr {size_bits};

    if (global_offset > size_bits) {
        return new_arr;
    }

    int sz = size_bytes_();
    for (int i = 0; i < sz - global_offset - 1; ++i) {
        new_arr.bytes.at(i) = (bytes.at(i + global_offset) >> offset_in_elem) | (bytes.at(i + global_offset + 1) << offset_in_elem);
    }
    new_arr.bytes.at(sz - global_offset - 1) = bytes.at(sz - 1) >> offset_in_elem;

    return new_arr;
}

BitArray &BitArray::set(int n, bool val) {
    int i = n / 8;
    if (n < 0) {
        throw std::out_of_range("Cannot set negative bit");
    }
    if (n >= size_bits) {
        throw std::out_of_range("Cannot set bit >= size");
    }
    reset(n);
    bytes[i] = bytes[i] | val << (n % 8);
    return *this;
}
BitArray &BitArray::set() {
    std::fill(bytes.begin(), bytes.end(), 255);
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
BitArray & BitArray::operator&=(const BitArray &b) {
    *this = *this & b;
    return *this;
}

BitArray & BitArray::operator|=(const BitArray &b) {
    *this = *this | b;
    return *this;
}

BitArray & BitArray::operator^=(const BitArray &b) {
    *this = *this ^ b;
    return *this;
}

BitArray & BitArray::operator<<=(int n) {
    *this = *this << n;
    return *this;
}

BitArray & BitArray::operator>>=(int n) {
    *this = *this >> n;
    return *this;
}

BitArray& BitArray::operator=(const BitArray &b) {
    if (&b != this) {
        bytes = b.bytes;
    }
    return *this;
}

void BitArray::resize(int num_bits, bool value) {
    if (num_bits < 0) {
        throw std::invalid_argument("Size can't be < 0");
    }
    int prev_sz_bits = size_bits;

    size_bits = num_bits;
    if (num_bits > prev_sz_bits) {
        for (int i = prev_sz_bits; i % 8 != 0 && i < num_bits; i++) {
            set(i, value);
        }
    }
    bytes.resize(size_bytes_());
    if (value != 0) {
        std::fill(bytes.begin() + prev_sz_bits / 8 + (prev_sz_bits % 8 != 0), bytes.end(), 255);
    }

}

bool BitArray::operator[](int i) const {
    return (bytes[i / 8] >> (i % 8)) & 1;
}

BitArray BitArray::operator~() const {
    auto new_arr = BitArray(*this);
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

BitArray operator&(const BitArray &b1, const BitArray &b2) {
    BitArray result = BitArray::do_bit_operation(b1, b2, operation::AND);
    return result;
}
BitArray operator|(const BitArray &b1, const BitArray &b2) {
    BitArray result = BitArray::do_bit_operation(b1, b2, operation::XOR);
    return result;
}
BitArray operator^(const BitArray &b1, const BitArray &b2) {
    BitArray result = BitArray::do_bit_operation(b1, b2, operation::OR);
    return result;
}

std::ostream& operator<<(std::ostream &lhs, const BitArray &b1) {
    return lhs << b1.to_string();
}



