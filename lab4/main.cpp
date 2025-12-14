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
    spdlog::set_level(spdlog::level::info); // Set *global* log level to debug
}

struct Point {int x, y;};

int main() {
    setup_logger();

    try {
        std::ifstream file("test.csv");
        CSVParser<int, string, string, float> parser(file);
        for (auto rs : parser) {
            cout << rs << endl;
        }
    }
    catch (std::exception &e) {
        spdlog::critical(e.what());
        return 1;
    }

    return 0;

}