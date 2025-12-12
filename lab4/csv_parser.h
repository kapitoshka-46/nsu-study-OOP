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
        char column_delimiter = ',';
        char line_delimiter = '\n'; // TODO: use line_delimiter for parsing
        char escape = '\\'; //TODO: use escape character for parsing
    };


    explicit CSVParser(const std::string &file, char delimiter =',', char quote = '"', int skip_lines = 0);

    explicit CSVParser(std::ifstream &&file,    char delimiter =',', char quote = '"', int skip_lines = 0);

    explicit CSVParser(std::ifstream &file,     char delimiter =',', char quote = '"', int skip_lines = 0); // may be delete ???

    explicit CSVParser(std::istream &is,        char delimiter =',', char quote = '"', int skip_lines = 0);

    CSVParser(const CSVParser &other) = delete;
    CSVParser &operator=(const CSVParser &other) = delete;

    CSVParser(CSVParser &&other)  noexcept = default;
    CSVParser &operator=(CSVParser &&other) noexcept = default;

    explicit operator bool() const noexcept {
        return is_good_;
    }

    CSVParser& operator>>(std::tuple<Args ...> &t) {
        if (istream_.fail()) {
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
    const char delimiter_;
    const char quote_;
    bool is_good_ = true;
    int line_ = 0;

    std::vector<std::string> GetRecordAndParse(); // TODO: сделать совместимость для  basic_string
};


// ---------------- Constructors --------------------
template<typename ... Args>
CSVParser<Args...>::CSVParser(const std::string &file, char delimiter, char quote, int skip_lines)
: owned_file_(std::make_unique<std::ifstream>(file)),
istream_(*owned_file_),
delimiter_(delimiter),
quote_(quote)
{
    spdlog::debug("CSVParser constructor");
    spdlog::info("Set delimiter: {}", delimiter_);
}

template<typename ... Args>
CSVParser<Args...>::CSVParser(std::ifstream &&file, char delimiter, char quote, int skip_lines)
    :owned_file_(std::make_unique<std::ifstream>(std::move(file))),
istream_(*owned_file_),
delimiter_(delimiter),
quote_(quote)
{
    spdlog::debug("CSVParser constructor");
    spdlog::info("Set delimiter: {}", delimiter_);
}

template<typename ... Args>
CSVParser<Args...>::CSVParser(std::ifstream &file, char delimiter, char quote, int skip_lines)
: istream_(file),
delimiter_(delimiter),
quote_(quote)
{
    spdlog::debug("CSVParser: constructor");
    spdlog::info("Set delimiter: \"{}\"", delimiter_);
}

template<typename ... Args>
CSVParser<Args...>::CSVParser(std::istream &is, char delimiter, char quote, int skip_lines)
    : istream_(is),
delimiter_(delimiter),
quote_(quote)
{
}



inline std::string read_quote_and_field_to_the_end(std::istream & is, char delimiter, char quote) {
    std::string res;
    std::getline(is, res, quote);
    std::string other;
    std::getline(is, other, delimiter);
    res += other;
    return res;
}


template <typename ... Args>
std::vector<std::string> CSVParser<Args...>::GetRecordAndParse() {
    /* TODO: rewrite it if you have time.
     * idea: 1) read full record from start to the end
     *       2) give it to function: parse_record(basic_string record, CharT delim, CharT quote)
    */
    line_++;
    using String = std::string;

    std::vector<String> result;

    String line;
    if (!std::getline(istream_, line)) {
        return result;
    }
    const String copy = line;
    spdlog::debug("CSVParser: parse csv line: {}", line);

    while (true) {
        spdlog::debug("current line: {}", line);
        auto quote_pos = line.find(quote_);
        auto delim_pos = line.find(delimiter_);

        if (quote_pos == delim_pos) {
            assert(quote_pos == String::npos && delim_pos == String::npos);
            if (!line.empty()) {result.push_back(line);}
            spdlog::debug("done");
            break;
        }
        if (quote_pos < delim_pos) {
            auto end_quote = line.find(quote_, quote_pos + 1);
            if (end_quote != std::string::npos) {
                // TODO: выглядит не очень ...
                line.erase(quote_pos, 1); end_quote--; delim_pos--;
                line.erase(end_quote, 1); end_quote--; delim_pos--;
            }
            else {
                auto prev_len = line.length();
                String tmp;
                std::getline(istream_, tmp, quote_);
                line += tmp += quote_;
                tmp.clear();

                if (istream_.eof()) {  // если не получилось сосчитать, значит до этого дочитали до конца!
                    throw ParseException(line, line_, quote_pos + 1, "Unclosed quote");
                }

                std::getline(istream_, tmp, delimiter_);

                line += tmp;
                spdlog::debug("fetching data to the next delimiter: {}", line.substr(prev_len));

                // if quote_pos < delim_pos it means that field starts from line[0]
                // and we need to push all data until another delimiter
                // also we read line util delimiter -> just push line
                line.erase(quote_pos, 1);
                delim_pos = line.length() - 1;
                std::getline(istream_, tmp); // дочитываем до новой строки
                line += tmp;
            }
            //aaa " aaaa "   , other  (1)
            //aaa " aaaa ,  " hhjl, other  (2)
            if (delim_pos < end_quote) {  // (2)
                    delim_pos = line.find(delimiter_, end_quote);
                    if (delim_pos == std::string::npos) {
                        delim_pos = line.length();
                    }
                }
            }
        result.push_back(line.substr(0, delim_pos));

        line = delim_pos == line.length() ? "" : line.substr(delim_pos + 1);
    }
    if (result.size() < sizeof...(Args)) {
        throw ParseException(copy, line_, copy.size() + 1, "Expected more fields");
    }
    if (result.size() > sizeof...(Args)) {
        throw ParseException(copy, line_, copy.size() - line.size() + 1, "Got unexpected extra fields");
    }

    return result;
}


template <typename ...Args>
class CSVParser<Args...>::InputIterator {
public:

    using iterator_category = std::input_iterator_tag;
    using value_type = std::tuple<Args...>;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type *;
    using reference = value_type &;
    using const_reference = const value_type &;

    InputIterator() = default;
    InputIterator(CSVParser *parser) : parser_(parser) {}

    reference operator*() {
        return value_;
    }
    const_reference operator*() const {
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


