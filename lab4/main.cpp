#include <iostream>
#include "tuple_extension.h"
#include <fstream>

#include "csv_parser.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

using namespace std;

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
        while (parser.Next()) {
            cout << parser.Value() << "\n";
        }
        cout << parser.Value() << endl;
        return 0;
    }
    catch (std::exception &e) {
        spdlog::critical(e.what());
    }
}