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
    // TODO округление интересное
    // TODO bit twiddle hacks
    if (value != 0) {
        for (int i = 0; i < bytes.size(); i++) {
            unsigned shift = i * SIZE_ELEM;
            if (shift >= sizeof(decltype(value)) * SIZE_ELEM) {
                // на самом деле процессоры делают value >> (shift % 64)
                // такой сдвиг приведёт к неккоректному заполнению массива
                // к тому же, value can't be bo
                break;
            }
            bytes[i] = static_cast<uchar>(value >> shift);
        }
    }
}


// ------------------------------ const methods ----------------------------- //
int BitArray::size() const {
    return size_bits;
}

bool BitArray::empty() const {
    if (size_bits == 0) {
        return true;
    }
    return false;
}

int BitArray::size_bytes_() const {
    int num_bytes = size_bits / SIZE_ELEM;
    if (size_bits % SIZE_ELEM != 0) {
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

BitArray BitArray::operator<<(int n) const {
    int offset_in_elem = n % SIZE_ELEM;
    int global_offset = n / SIZE_ELEM;
    BitArray new_arr {size_bits};

    if (global_offset > size_bits) {
        return new_arr;
    }

    new_arr.bytes.at(global_offset) = bytes.at(0) << offset_in_elem;
    int sz_bytes = size_bytes_();
    for (int i = global_offset + 1; i < sz_bytes; ++i) {
        // let shift = 3
        // ______xxx from prev byte
        // xxxxxx___ from current byte
        auto from_prev = bytes.at(i - global_offset - 1) >> (SIZE_ELEM - offset_in_elem);
        auto from_new = bytes.at(i - global_offset) << offset_in_elem;
        new_arr.bytes.at(i) = from_prev | from_new;
    }
    return new_arr;
}


BitArray BitArray::operator>>(int n) const {
    int offset_in_elem = n % SIZE_ELEM;
    int global_offset = n / SIZE_ELEM;
    BitArray new_arr {size_bits};

    if (global_offset > size_bits) {
        return new_arr;
    }

    int sz = size_bytes_();
    for (int i = 0; i < sz - global_offset - 1; ++i) {
        auto from_next = bytes.at(i+global_offset + 1) << (SIZE_ELEM - offset_in_elem);
        auto from_curr = bytes.at(i + global_offset) >> offset_in_elem;
        new_arr.bytes.at(i) = from_next | from_curr;
    }
    new_arr.bytes.at(sz-global_offset - 1) = bytes.at(sz - 1) >> offset_in_elem;
    return new_arr;
}

BitArray BitArray::do_bit_operation(const BitArray &a, const BitArray &b, const operation op) {
    if (a.size_bits != b.size_bits) {
        throw std::invalid_argument("The arrays must be the shame size");
    }
    int sz = a.size_bits;
    BitArray result {};
    result.bytes.reserve(a.size_bytes_());
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
        for (int i = sz - sz % SIZE_ELEM; i < sz; i++) {
            result.reset(i - 1);
        }
    }
    return result;
}

// ------------------------------- methods ------------------------------ //
void BitArray::swap(BitArray &other) {
    bytes.swap(other.bytes);

    int tmp_sz = size_bits;
    size_bits = other.size_bits;
    other.size_bits = tmp_sz;
}

void BitArray::clear() {
    bytes.clear();
    size_bits = 0;
}

BitArray &BitArray::reset(int n) {
    if (n < 0) {
        throw std::out_of_range("Cannot reset negative bit");
    }
    int i = n / SIZE_ELEM;
    auto mask = ~(1 << (n % SIZE_ELEM));
    bytes[i] = bytes[i]  &  mask;
    return *this;
}
BitArray &BitArray::reset() {
    std::fill(bytes.begin(), bytes.end(),0);
    return *this;
}

BitArray &BitArray::set(int n, bool val) {
    int i = n / SIZE_ELEM;
    if (n < 0) {
        throw std::out_of_range("Cannot set negative bit");
    }
    if (n >= size_bits) {
        throw std::out_of_range("Cannot set bit >= size");
    }
    reset(n);
    bytes[i] = bytes[i] | val << (n % SIZE_ELEM);
    return *this;
}
BitArray &BitArray::set() {
    std::fill(bytes.begin(), bytes.end(), (2 << SIZE_ELEM) - 1);
    return *this;
}


void BitArray::push_back(bool bit) {
    int prev_size_bits = size_bits;
    size_bits += 1;
    if (prev_size_bits % SIZE_ELEM == 0) {
        bytes.push_back(bit);
    }
    else {
        set(prev_size_bits, bit);
    }

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
        size_bits = b.size_bits;
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
        for (int i = prev_sz_bits; i % SIZE_ELEM != 0 && i < num_bits; i++) {
            set(i, value);
        }
    }
    bytes.resize(size_bytes_());
    if (value != 0) {
        std::fill(bytes.begin() + prev_sz_bits / SIZE_ELEM + (prev_sz_bits % SIZE_ELEM != 0), bytes.end(), (2 << SIZE_ELEM) - 1);
    }

}

bool BitArray::get(int i) const{
    if (i < 0 or i >= size_bits) {
        throw std::out_of_range("Can't access the element not in range of BitArray");
    }
    return (bytes[i / SIZE_ELEM] >> (i % SIZE_ELEM)) & 1;
}
bool BitArray::operator[](int i) const {
    if (i < 0 or i >= size_bits) {
        throw std::out_of_range("Can't access the element not in range of BitArray");
    }
    return get(i);
}

proxyAt BitArray::operator[](int i) {
    if (i < 0 or i >= size_bits) {
        throw std::out_of_range("i <= 0 or i >= size");
    }
    return proxyAt {*this, i};
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
    BitArray result = BitArray::do_bit_operation(b1, b2, operation::OR);
    return result;
}
BitArray operator^(const BitArray &b1, const BitArray &b2) {
    BitArray result = BitArray::do_bit_operation(b1, b2, operation::XOR);
    return result;
}

std::ostream& operator<<(std::ostream &lhs, const BitArray &b1) {
    return lhs << b1.to_string();
}

proxyAt::proxyAt(BitArray& arr, int index): arr(arr), index(index) {};

proxyAt &proxyAt::operator=(bool bit) {
    arr.set(index, bit);
    return *this;
}

proxyAt::operator bool() const {
    return arr.get(index);
}



