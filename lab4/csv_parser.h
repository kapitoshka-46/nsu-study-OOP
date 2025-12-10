#ifndef CSV_PARSER_H
#define CSV_PARSER_H
#include <fstream>
#include <memory>
#include <string>
#include <sstream>
#include <tuple>
#include <spdlog/spdlog.h>
#include "tuple_extension.h"
#include <gtest/gtest.h>

#include "test/color.h"

// ------------------- Exceptions ----------------- //
struct CSVParserException : std::runtime_error {
    explicit CSVParserException(const std::string &msg)
    : std::runtime_error("CSVParser: " + msg) {};
};


struct ParseException : CSVParserException {
    explicit ParseException(
        const std::string &line,
        int row, int column,
        const std::string &msg
        )
    : CSVParserException(
    "error: " + color::red + msg + color::reset + " at line " + std::to_string(row)
    + ", symbol " + std::to_string(column) + "\n"
    + line + "\n"
    + std::string(column - 1, ' ') + color::red + "^" + color::reset)
    {}
};


template <typename ...Args>
class CSVParser {
public:
    explicit CSVParser(const std::string &file, char delimiter=',', int skip_lines=0);

    explicit CSVParser(std::ifstream &&file, char delimiter=',', int skip_lines=0);

    explicit CSVParser(std::ifstream &file, char delimiter=',', int skip_lines=0); // may be delete ???

    explicit CSVParser(std::istream &is, char delimiter=',', int skip_lines=0);

    CSVParser(const CSVParser &other) = delete;
    CSVParser &operator=(const CSVParser &other) = delete;

    CSVParser(CSVParser &&other)  noexcept = default;
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
    spdlog::debug("CSVParser: constructor");
    spdlog::info("Set delimiter: \"{}\"", delimiter_);
}

template<typename ... Args>
CSVParser<Args...>::CSVParser(std::istream &is, char delimiter, int skip_lines)
    : stream_(is), delimiter_(delimiter) {
}

inline std::string read_quote_and_field_to_the_end(std::istream & is, char delimiter, char quote) {
    std::string res;
    std::getline(is, res, quote);
    std::string other;
    std::getline(is, other, delimiter);
    res += other;
    return res;
}

template <class CharT=char, class Traits=std::char_traits<CharT>>
std::vector<std::basic_string<CharT, Traits>>
parse_csv_line(std::basic_istream<CharT, Traits> &is, CharT delimiter, CharT quote, int expected_fields=-1) {
    static int count = 0; count++;

    using String = std::basic_string<CharT, Traits>;

    std::vector<String> result;

    String line;
    if (!std::getline(is, line)) {
        return result;
    }
    String copy = line;
    spdlog::debug("CSVParser: parse csv line: {}", line);

    while (true) {
        spdlog::debug("current line: {}", line);
        auto quote_pos = line.find(quote);
        auto delim_pos = line.find(delimiter);

        if (quote_pos == delim_pos) {
            assert(quote_pos == String::npos && delim_pos == String::npos);
            if (!line.empty()) {result.push_back(line);}
            break;
        }
        if (quote_pos < delim_pos) {
            auto end_quote = line.find(quote, quote_pos + 1);

            if (end_quote == std::string::npos) {
                auto prev_len = line.length();
                String tmp;
                std::getline(is, tmp, quote);
                line += tmp;
                std::getline(is, tmp, delimiter);
                line += tmp;
                spdlog::debug("fetching data to the next delimiter: {}", line.substr(prev_len));

                // if quote_pos < delim_pos it means that field starts from line[0]
                // and we need to push all data until another delimiter
                // also we read line util delimiter -> just push line
                result.push_back(line);
                line.clear();
                std::getline(is, line); // дочитываем до новой строки
                continue;
            }

            if (end_quote != std::string::npos) { //aaa " aaaa "   , other  (1)
                                                  //aaa " aaaa ,  " hhjl, other  (2)
                if (end_quote > delim_pos) {  // (2)
                    delim_pos = line.find(delimiter, end_quote);
                    if (delim_pos == std::string::npos) {
                        delim_pos = line.length() - 1;
                    }
                }
            }

        }
        result.push_back(line.substr(0, delim_pos));
        line = line.substr(delim_pos + 1);
    }
    if (expected_fields != -1 && result.size() < expected_fields) {
        throw ParseException(copy, count, copy.size() + 1, "Expected more fields");
    }
    if (expected_fields != -1 && result.size() > expected_fields) {
        throw ParseException(copy, count, copy.size() - line.size() + 1, "Got unexpected extra fields");
    }
    return result;
}

template<class ... Args>
bool CSVParser<Args...>::Next() {
    spdlog::debug("CSVParser: Next()");

    if (stream_.fail()) {
        return false;
    }

    std::vector<std::string> fields = parse_csv_line(stream_, delimiter_, '"', sizeof...(Args));
    if (fields.empty()) {
        return false;
    }

    value_ = make_tuple_from_csv_fields<char, std::char_traits<char>,Args...>(fields);
    return true;
}

template<class ... Args>
const std::tuple<Args...> & CSVParser<Args...>::Value() const {
    return value_;  // TODO: что делать, если еще не было вызова Next() ?????????
}





#endif //CSV_PARSER_H


