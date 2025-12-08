#include <iostream>
#include "tuple_extension.h"
#include <fstream>

#include "csv_parser.h"

using namespace std;
int main() {
    std::ifstream file("test.csv");
    CSVParser<int, string> parser(file, 0 /*skip first lines count*/);
    // for (tuple<int, string> rs : parser) {
    //     cout<<rs<<endl; }

    while (parser.Next()) {
        cout << parser.Value() << "\n";
    }
    cout << parser.Value() << endl;
    return 0;

    // std::tuple<int, float, std::string> t {3, 2.5, "kek"};
    // std::cout << t <<'\n';

}