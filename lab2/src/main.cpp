#include <iostream>
#include <vector>

#include "Core.h"

using namespace std;
using enum CellType;
int main() {
    string s = "-----"
               "-##--"
               "-#-#-"
               "--#--"
               "-----";
    Field field {s, 5, 5};
    cout << field.ToString() << endl;
    cout << (field.GetCellState({1,1}) == kAlive) << endl;
    return 0;
}