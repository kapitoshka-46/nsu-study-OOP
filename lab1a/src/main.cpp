#include <iostream>
#include <bitset>
#include <boost/dynamic_bitset/dynamic_bitset.hpp>

#include "BitArray.h"

// сделать. proxy_class т.к. бит не адресуем.
// TODO b[i] = true
// for (auto b : BitArrayObject)

int main() {
    using namespace std;
    // using boost::dynamic_bitset;
    // int sz1 = 32;
    // int sz2 = 32;
    // unsigned long value1 = 1;
    // unsigned long value2 = 1;
    // BitArray b1(sz1, value1);
    // BitArray b2 {sz2, value2};
    // dynamic_bitset<> d1 (sz1, value1);
    // dynamic_bitset<> d2 (sz2, value2);
    //
    // unsigned shift = 8;
    // cout << "-------init --------" << endl;
    // cout << b1 << endl;
    // cout << "------- my --------" << endl;


    // cout << "------boost------" << endl;


    BitArray ba {3, 0};
    cout << ba << endl;
    ba[1] = true;
    bool bit = ba[1];
    cout << ba[1] << endl;
    return 0;
}