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
    explicit CSVParser(const std::string &file, char delimiter =',', char quote = '"', int skip_lines = 0);

    explicit CSVParser(std::ifstream &&file,    char delimiter =',', char quote = '"', int skip_lines = 0);

    explicit CSVParser(std::ifstream &file,     char delimiter =',', char quote = '"', int skip_lines = 0); // may be delete ???

    explicit CSVParser(std::istream &is,        char delimiter =',', char quote = '"', int skip_lines = 0);

    CSVParser(const CSVParser &other) = delete;
    CSVParser &operator=(const CSVParser &other) = delete;

    CSVParser(CSVParser &&other)  noexcept = default;
    CSVParser &operator=(CSVParser &&other) noexcept = default;

    bool Next();
    const std::tuple<Args ...> &Value() const;

private:
    std::unique_ptr<std::ifstream> owned_file_ {nullptr};
    std::istream &istream_;
    std::tuple<Args...> value_;
    const char delimiter_;
    const char quote_;
    int line_ = 1;

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

    using String = std::string;

    std::vector<String> result;

    String line;
    if (!std::getline(istream_, line)) {
        return result;
    }
    String copy = line;
    spdlog::debug("CSVParser: parse csv line: {}", line);

    while (true) {
        spdlog::debug("current line: {}", line);
        auto quote_pos = line.find(quote_);
        auto delim_pos = line.find(delimiter_);

        if (quote_pos == delim_pos) {
            assert(quote_pos == String::npos && delim_pos == String::npos);
            if (!line.empty()) {result.push_back(line);}
            break;
        }
        if (quote_pos < delim_pos) {
            auto end_quote = line.find(quote_, quote_pos + 1);
            if (end_quote != std::string::npos) {
                line.erase(quote_pos, 1);
                line.erase(end_quote, 1);
            }
            else {
                auto prev_len = line.length();
                String tmp;
                std::getline(istream_, tmp, quote_);
                line += tmp;
                tmp.clear();
                std::getline(istream_, tmp, delimiter_);
                if (istream_.eof()) {  // если не получилось сосчитать, значит до этого дочитали до конца!
                    throw ParseException(line, line_, quote_pos + 1, "Unclosed quote");
                }
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
                        delim_pos = line.length() - 1;
                    }
                }
            }
        result.push_back(line.substr(0, delim_pos));
        line = line.substr(delim_pos + 1);
    }
    if (result.size() < sizeof...(Args)) {
        throw ParseException(copy, line_, copy.size() + 1, "Expected more fields");
    }
    if (result.size() > sizeof...(Args)) {
        throw ParseException(copy, line_, copy.size() - line.size() + 1, "Got unexpected extra fields");
    }

    line_++;  // нельзя делать статической !!!!!!!!!!! сохраняется состояние между объектами
    return result;
}

template<class ... Args>
bool CSVParser<Args...>::Next() {
    spdlog::debug("CSVParser: Next()");

    if (istream_.fail()) {
        return false;
    }

    std::vector<std::string> fields = GetRecordAndParse();
    if (fields.empty()) {
        return false;
    }

    value_ = make_tuple_from_strings<char, std::char_traits<char>,Args...>(fields);
    return true;
}

template<class ... Args>
const std::tuple<Args...> & CSVParser<Args...>::Value() const {
    return value_;  // TODO: что делать, если еще не было вызова Next() ?????????
}





#endif //CSV_PARSER_H


