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

    //auto logger = spdlog::basic_logger_mt("file_logger", "app.log");

    // // Делаем его логгером по умолчанию (чтобы spdlog::info() писал в файл)
    //spdlog::set_default_logger(logger);

}
int main() {
    setup_logger();
    std::ifstream file("test.csv");

    CSVParser<int, string, string> parser(file, ',', 0 /*skip first lines count*/);

    try {
        string record = "a,b,\"ccc,\n\n cc,\n c\ncc\"";
        istringstream iss(record);
        CSVParser<string, string, string> csv {iss};

        TripleString expected = {"a","b","ccc, cc, ccc"};
        TripleString t;
        csv >> t;

    }
    catch (std::exception &e) {
        spdlog::critical(e.what());
    }
    return 0;

}