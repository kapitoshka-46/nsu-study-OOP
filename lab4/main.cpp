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
    std::ifstream file("test.csv");


    try {
        basic_ostringstream<wchar_t> oss;
       // std::tuple<Point, int> t({3, 4}, 5);
        std::tuple<int, int> t(1, 2);
        oss  << t  << endl;
    }
    catch (std::exception &e) {
        spdlog::critical(e.what());
    }
    return 0;

}