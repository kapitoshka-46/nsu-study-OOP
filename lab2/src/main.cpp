#include <iostream>
#include <vector>

#include "Core.h"

using namespace std;
int main() {
    string s = "-----"
               "-##--"
               "-#-#-"
               "--#--"
               "-----";
    Field field {s, 5, 5};
    cout << field.ToString() << endl;
    return 0;
}