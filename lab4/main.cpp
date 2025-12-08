#include <iostream>
#include "tuple_extension.h"

int main() {
    std::tuple<int, float, double, std::string> t {3, 2.5f, 1.5f, "string"};
    std::cout << t;
    return 0;
}