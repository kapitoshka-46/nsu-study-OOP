#include <iostream>
#include "tuple_extension.h"
#include <fstream>

#include "csv_parser.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

using namespace std;
using TripleString = tuple<string,string, string>;

void setup_logger() {
    spdlog::set_pattern("[%^%l%$] %v");
    spdlog::set_level(spdlog::level::debug); // Set *global* log level to debug
}

struct Point {int x, y;};

int main() {
    setup_logger();
    vector<tuple<int, string, string, float>> tuples;
    int i = 0;
    tuples.resize(40000);
    try {
        std::ifstream file("test_big.csv");
        CSVParser<int, string, string, float> parser(file);
        while (parser >> tuples.at(i++)) {};
    }
    catch (std::exception &e) {
        spdlog::critical(e.what());
    }

    cout << tuples.at(1) << "\n";
    cout << tuples.at(2) << "\n";
    cout << tuples.at(3) << "\n";
    cout << "..." << "\n";
    cout << tuples.at(i - 3) << "\n";
    cout << tuples.at(i - 2) << "\n";
    return 0;

}