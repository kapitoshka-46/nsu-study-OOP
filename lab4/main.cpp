#include <iostream>
#include "tuple_extension.h"

int main() {
    std::tuple<int, float, double, std::string> t {3, 2.5f, 1.5f, "lol kek cheburek"};
    std::cout << t;
    return 0;
}