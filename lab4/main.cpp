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

int main() {
    setup_logger();
    std::ifstream file("test.csv");


    try {
        // string record = "1, 11\n"
        //                 "2, 22\n"
        //                 "3, 33\n";
        string record = "1, 11\n";
        istringstream iss(record);
        CSVParser<int, int> csv {iss};
        for (auto t : csv) {
            cout << t << endl;
        }
    }
    catch (std::exception &e) {
        spdlog::critical(e.what());
    }
    return 0;

}