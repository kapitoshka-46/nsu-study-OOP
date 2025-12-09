#ifndef CSV_PARSER_H
#define CSV_PARSER_H
#include <fstream>
#include <memory>
#include <string>
#include <sstream>
#include <tuple>
#include <spdlog/spdlog.h>

#include "tuple_extension.h"

template <typename ...Args>
class CSVParser {
public:
    explicit CSVParser(const std::string &file, char delimiter=',', int skip_lines=0);

    explicit CSVParser(std::ifstream &&file, char delimiter=',', int skip_lines=0);

    explicit CSVParser(std::ifstream &file, char delimiter=',', int skip_lines=0);

    CSVParser(const CSVParser &other) = delete;
    CSVParser &operator=(const CSVParser &other) = delete;

    CSVParser(const CSVParser &&other)  noexcept = default;
    CSVParser &operator=(CSVParser &&other) noexcept = default;

    bool Next();
    const std::tuple<Args ...> &Value() const;

private:
    std::unique_ptr<std::ifstream> owned_file_ {nullptr};
    std::istream &stream_;
    std::tuple<Args...> value_;
    const char delimiter_;
};


// ---------------- Constructors --------------------
template<typename ... Args>
CSVParser<Args...>::CSVParser(const std::string &file, char delimiter, int skip_lines)
: owned_file_(std::make_unique<std::ifstream>(file)),
stream_(*owned_file_),
delimiter_(delimiter) {
    spdlog::debug("CSVParser constructor");
    spdlog::info("Set delimiter: {}", delimiter_);
}

template<typename ... Args>
CSVParser<Args...>::CSVParser(std::ifstream &&file, char delimiter, int skip_lines)
    :owned_file_(std::make_unique<std::ifstream>(std::move(file))),
stream_(*owned_file_),
delimiter_(delimiter) {
    spdlog::debug("CSVParser constructor");
    spdlog::info("Set delimiter: {}", delimiter_);
}

template<typename ... Args>
CSVParser<Args...>::CSVParser(std::ifstream &file, char delimiter, int skip_lines)
    : stream_(file),
delimiter_(delimiter) {
    spdlog::debug("CSVParser constructor");
    spdlog::info("Set delimiter: \"{}\"", delimiter_);
}


template <class CharT, class Traits>
std::vector<std::basic_string<CharT, Traits>> parse_csv_line(std::basic_string<CharT, Traits> line, CharT delimiter, CharT quote) {
    using String = std::basic_string<CharT, Traits>;
    std::istringstream iss(line);
    // 1st, 2nd, 3rd, "4th, 4th", 5th, 6th
    String field;
    std::vector<String> result;

    auto start = 0;
    auto end = 0;

    while (end < line.length() - 1) {
        if (line.at(start) == quote) {
            start++; // не хотим учиытвать кавычку
            end = line.find(quote, start);
            if (end == String::npos) {
                throw std::runtime_error("CSV parser does not contain quote");
            }
        }
        else {
            end = line.find(delimiter, start + 1);
            if (end == String::npos) {
                end = line.length();
            }
        }


        result.push_back(line.substr(start, end - start));
        start = end + 1;
    }


    return result;
}

template<typename ... Args>
bool CSVParser<Args...>::Next() {
    spdlog::debug("CSVParser<Args...>::Next()");

    if (!stream_) {
        return false;
    }

    std::string line;
    std::getline(stream_, line);
    const std::vector<std::string> fields = parse_csv_line(line, delimiter_, '"');


    value_ = make_tuple_from_csv_fields<char, std::char_traits<char>,Args...>(fields);
    return stream_.good();
}

template<typename ... Args>
const std::tuple<Args...> & CSVParser<Args...>::Value() const {
    return value_;
}


#endif //CSV_PARSER_H
