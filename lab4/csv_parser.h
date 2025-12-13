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
        size_t row, size_t column,
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

    class InputIterator;

    struct Config {
        char delimiter;
        char line_delimiter;
        char quote;
        char escape; //TODO: use escape character for parsing
    };


    explicit CSVParser(const std::string &file, int skip_lines = 0, char delimiter =',', char line_delimiter='\n', char quote = '"', char escape='\0');

    explicit CSVParser(std::ifstream &&file,    int skip_lines = 0, char delimiter =',', char line_delimiter='\n', char quote = '"', char escape='\0');

    explicit CSVParser(std::ifstream &file,     int skip_lines = 0, char delimiter =',', char line_delimiter='\n', char quote = '"', char escape='\0'); // may be delete ???

    explicit CSVParser(std::istream &is,        int skip_lines = 0, char delimiter =',', char line_delimiter='\n', char quote = '"', char escape='\0');

    CSVParser(const CSVParser &other) = delete;
    CSVParser &operator=(const CSVParser &other) = delete;

    CSVParser(CSVParser &&other)  noexcept = default;
    CSVParser &operator=(CSVParser &&other) noexcept = default;

    explicit operator bool() const noexcept {
        return is_good_;
    }

    CSVParser& operator>>(std::tuple<Args ...> &t) {
        if (!istream_) {
            is_good_ = false;
            return *this;
        }


        std::vector<std::string> fields = GetRecordAndParse();
        if (fields.empty()) {
            is_good_ = false;
            return *this;
        }

        t = make_tuple_from_strings<char, std::char_traits<char>,Args...>(fields);
        return *this;
    }

    // iterators methods
    InputIterator begin() {
        return InputIterator(this);
    }
    InputIterator end() {
        return InputIterator();
    }

private:
    std::unique_ptr<std::ifstream> owned_file_ {nullptr};
    std::istream &istream_;
    Config cfg_;
    bool is_good_ = true;
    int line_ = 0;

    void SkipLines(int n);
    std::vector<std::string> GetRecordAndParse(); // TODO: сделать совместимость для  basic_string
};


// ---------------- Constructors --------------------
template<typename ... Args>
CSVParser<Args...>::CSVParser(const std::string &file, int skip_lines, char delimiter, char line_delimiter, char quote, char escape)
: owned_file_(std::make_unique<std::ifstream>(file)),
istream_(*owned_file_),
cfg_(delimiter, line_delimiter, quote, escape)
{
    SkipLines(skip_lines);
    spdlog::info("CSVParser: skipping {} lines", skip_lines);
    spdlog::debug("CSVParser(const std::string &file...) constructor");
    spdlog::info("Set delimiter: {}", cfg_.delimiter);
}

template<typename ... Args>
CSVParser<Args...>::CSVParser(std::ifstream &&file, int skip_lines, char delimiter, char line_delimiter, char quote, char escape)
    :owned_file_(std::make_unique<std::ifstream>(std::move(file))),
istream_(*owned_file_),
cfg_(delimiter, line_delimiter, quote, escape)
{
    SkipLines(skip_lines);
    spdlog::info("CSVParser: skipping {} lines", skip_lines);
    spdlog::debug("CSVParser(std::ifstream &&file, ...) constructor");
    spdlog::info("Set delimiter: {}", cfg_.delimiter);
}

template<typename ... Args>
CSVParser<Args...>::CSVParser(std::ifstream &file, int skip_lines, char delimiter, char line_delimiter, char quote, char escape)
: istream_(file),
cfg_(delimiter, line_delimiter, quote, escape)
{
    SkipLines(skip_lines);
    spdlog::info("CSVParser: skipping {} lines", skip_lines);
    spdlog::debug("CSVParser: (std::ifstream &file,...) constructor");
    spdlog::info("Set delimiter: \"{}\"", cfg_.delimiter);
}

template<typename ... Args>
CSVParser<Args...>::CSVParser(std::istream &is, int skip_lines, char delimiter, char line_delimiter, char quote, char escape)
    : istream_(is),
cfg_(delimiter, line_delimiter, quote, escape)
{
    SkipLines(skip_lines);
    spdlog::info("CSVParser: skipping {} lines", skip_lines);
    spdlog::debug("CSVParser: (std::istream &is, ...) constructor");
    spdlog::info("Set delimiter: \"{}\"", cfg_.delimiter);
}


template<typename ... Args>
void CSVParser<Args...>::SkipLines(int n) {
    std::tuple<Args ...> dummy;
    for (int i = 0; static_cast<bool>(*this) && i < n; ++i) {
        *this >> dummy;
    }
}

// template <typename ... Args>
// std::vector<std::string> CSVParser<Args...>::GetRecordAndParse() {
//     line_++;
//     using String = std::string;
//
//     std::vector<String> record;
//
//     String field;
//     char c;
//     bool in_quote = false;
//     while (istream_.read(&c, sizeof(c))) {
//         if (c == cfg_.quote) {
//             in_quote = !in_quote;
//             continue;
//         }
//         if (!in_quote && c == cfg_.delimiter) {
//             record.push_back(field);
//             field.clear();
//             continue;
//         }
//         if (!in_quote && c == cfg_.line_delimiter) {
//             line_++;
//             break;
//         }
//         field += c;
//     }
//     if (c == 0) {
//         is_good_ = false;
//         return record;
//     }
//     record.push_back(field);
//     field.clear();
//
//     if (record.size() < sizeof...(Args)) {
//         throw ParseException(field, line_, field.size() + 1, "Expected more fields");
//     }
//     if (record.size() > sizeof...(Args)) {
//         throw ParseException(field, line_, field.size() + 1, "Got unexpected extra fields");
//     }
//
//     return record;
// }

template <typename ... Args>
std::vector<std::string> CSVParser<Args...>::GetRecordAndParse() {
    using String = std::string;

    std::array<char, 1024> buff{}; // TODO
    size_t index = 0;

    std::vector<String> record;

    char c;
    bool in_quote = false;
    bool is_start = false;
    int field_length = 0;
    while (istream_.read(&c, sizeof(c))) {
        is_start = true;
        if (c == cfg_.quote) {
            in_quote = !in_quote;
            continue;
        }
        if (!in_quote && c == cfg_.delimiter) {

            record.emplace_back(buff.begin() + index - field_length, buff.begin() + index);
            field_length = 0;
            continue;
        }
        if (!in_quote && c == cfg_.line_delimiter) {
            line_++;
            break;
        }
        field_length++;
        buff.at(index++) = c;
        if (index == buff.size()) {
            index = 0;
        }

    }
    if (!istream_ && !is_start) {
        is_good_ = false;
        return record;
    }

    record.emplace_back(buff.begin() + index - field_length, buff.begin() + index);


    if (record.size() < sizeof...(Args)) {
        throw ParseException("ss", line_, 2, "Expected more fields");
    }
    if (record.size() > sizeof...(Args)) {
        throw ParseException("ss", line_, 2, "Got unexpected extra fields");
    }

    return record;
}


template <typename ...Args>
class CSVParser<Args...>::InputIterator {
public:

    using iterator_category = std::input_iterator_tag;
    using value_type = std::tuple<Args...>;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type *;
    using reference = value_type &;

    InputIterator() = default;
    explicit InputIterator(CSVParser *parser) : parser_(parser) {
        if (parser) {
            *parser_ >> value_;
        }
    }

    reference operator*() {
        return value_;
    }


    InputIterator &operator++() {
        if (parser_ == nullptr) {
            return *this;
        }

        if (!(*parser_ >> value_)) {
            parser_ = nullptr;
        }
        return *this;

    }

    InputIterator &operator++(int) {
        InputIterator &tmp = *this;
        ++(*this);
        return tmp;
    }


    friend bool operator==(const InputIterator &lhs, const InputIterator &rhs) {
        return lhs.parser_ == rhs.parser_;
    }


private:
    CSVParser *parser_;
    std::tuple<Args...> value_;
};



#endif //CSV_PARSER_H


