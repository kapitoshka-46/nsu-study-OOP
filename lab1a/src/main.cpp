#include <iostream>
#include <bitset>
#include "BitArray.h"



int main() {
    using namespace std;
    BitArray bitarr(8, 5);
    cout << bitarr << endl;

    bitarr.push_back(true);
    cout << bitarr << endl;
    cout << bitarr.size() << endl;


    cout << "------boost------" << endl;
    bitset<5> carr(15);
    return 0;
}