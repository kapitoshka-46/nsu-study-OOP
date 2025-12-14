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
    + std::string(std::max(column - 3, 0UL), ' ') + color::red + "^" + color::reset)
    {}
};

template <size_t N, typename CharT, typename Traits=std::char_traits<CharT>>
class CharBuffer {
public:

    explicit CharBuffer(std::basic_istream<CharT, Traits> &istream) : istream_(istream) {
        Refill();
        spdlog::debug("CharBuffer: (const basic_istream &) constructed");
    }
    explicit CharBuffer(std::basic_istream<CharT, Traits> &&istream) = delete;

    CharBuffer(const CharBuffer &other) = delete;
    CharBuffer &operator=(const CharBuffer &other) = delete;

    CharBuffer(CharBuffer &&other) = delete;
    CharBuffer &operator=(CharBuffer &&other) = delete;

    CharBuffer &operator>>(CharT &c) {
        if (pos_ == available_) { Refill(); }
        if (!IsGood()) { return *this; }

        c = data_.at(pos_++);
        return *this;
    }

    bool IsGood() const { return available_ > 0; }
    explicit operator bool() const { return IsGood(); }

private:
    void Refill() {
        pos_ = 0;
        istream_.read(reinterpret_cast<CharT *>(data_.data()), N * sizeof(CharT));
        available_ = istream_.gcount();
    }

    std::basic_istream<CharT, Traits> &istream_;
    size_t available_ = 0;
    size_t pos_ = available_;
    std::array<CharT, N> data_ {};
};


template <typename CharT, typename Traits, typename ...Args>
class BasicCSVParser {
public:

    class InputIterator;

    struct Config {
        CharT delimiter;
        CharT line_delimiter;
        CharT quote;
        CharT escape; //TODO: use escape character for parsing
    };


    explicit BasicCSVParser(const std::basic_string<CharT, Traits> &file, int skip_lines = 0, char delimiter =',', char line_delimiter='\n', char quote = '"', char escape='\0');

    explicit BasicCSVParser(std::basic_ifstream<CharT, Traits> &&file,    int skip_lines = 0, char delimiter =',', char line_delimiter='\n', char quote = '"', char escape='\0');

    explicit BasicCSVParser(std::basic_ifstream<CharT, Traits> &file,     int skip_lines = 0, char delimiter =',', char line_delimiter='\n', char quote = '"', char escape='\0'); // may be delete ???

    explicit BasicCSVParser(std::basic_istream<CharT, Traits> &is,        int skip_lines = 0, char delimiter =',', char line_delimiter='\n', char quote = '"', char escape='\0');

    BasicCSVParser(const BasicCSVParser &other) = delete;
    BasicCSVParser &operator=(const BasicCSVParser &other) = delete;

    BasicCSVParser(BasicCSVParser &&other)  noexcept = default;
    BasicCSVParser &operator=(BasicCSVParser &&other) noexcept = default;

    explicit operator bool() const noexcept {
        return is_good_;
    }

    BasicCSVParser& operator>>(std::tuple<Args ...> &t) {
        if (!buff_) {
            is_good_ = false;
            return *this;
        }


        std::vector<std::basic_string<CharT, Traits>> fields = GetRecordAndParse();
        if (fields.empty()) {
            is_good_ = false;
            return *this;
        }

        t = make_tuple_from_strings<Args...>(fields.begin(), fields.end());
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
    std::unique_ptr<std::basic_ifstream<CharT, Traits>> owned_file_ {nullptr};
    std::basic_istream<CharT, Traits> &istream_;
    CharBuffer<256, CharT, Traits> buff_ {istream_};

    Config cfg_;
    bool is_good_ = true;
    int line_ = 1;

    void SkipLines(int n);

    std::vector<std::basic_string<CharT, Traits>> GetRecordAndParse(); // TODO: сделать совместимость для  basic_string
};

template <typename ...Args>
using CSVParser = BasicCSVParser<char,std::char_traits<char>, Args ...>;


// ---------------- Constructors --------------------
template<typename CharT, typename Traits, typename ... Args>
BasicCSVParser<CharT, Traits, Args...>::BasicCSVParser(const std::basic_string<CharT, Traits> &file, int skip_lines, char delimiter, char line_delimiter, char quote, char escape)
: owned_file_(std::make_unique<std::ifstream>(file)),
istream_(*owned_file_),
cfg_(delimiter, line_delimiter, quote, escape)
{
    SkipLines(skip_lines);
    spdlog::info("CSVParser: skipping {} lines", skip_lines);
    spdlog::debug("CSVParser(const std::string &file...) constructor");
}

template<typename CharT, typename Traits, typename ... Args>
BasicCSVParser<CharT, Traits, Args...>::BasicCSVParser(std::basic_ifstream<CharT, Traits> &&file, int skip_lines, char delimiter, char line_delimiter, char quote, char escape)
    :owned_file_(std::make_unique<std::ifstream>(std::move(file))),
istream_(*owned_file_),
cfg_(delimiter, line_delimiter, quote, escape)
{
    SkipLines(skip_lines);
    spdlog::info("CSVParser: skipping {} lines", skip_lines);
    spdlog::debug("CSVParser(std::ifstream &&file, ...) constructor");
}

template<typename CharT, typename Traits, typename ... Args>
BasicCSVParser<CharT, Traits, Args...>::BasicCSVParser(std::basic_ifstream<CharT, Traits> &file, int skip_lines, char delimiter, char line_delimiter, char quote, char escape)
: istream_(file),
cfg_(delimiter, line_delimiter, quote, escape)
{
    SkipLines(skip_lines);
    spdlog::info("CSVParser: skipping {} lines", skip_lines);
    spdlog::debug("CSVParser: (std::ifstream &file,...) constructor");
}

template<typename CharT, typename Traits, typename ... Args>
BasicCSVParser<CharT, Traits, Args...>::BasicCSVParser(std::basic_istream<CharT, Traits> &is, int skip_lines, char delimiter, char line_delimiter, char quote, char escape)
    : istream_(is),
cfg_(delimiter, line_delimiter, quote, escape)
{
    SkipLines(skip_lines);
    spdlog::info("CSVParser: skipping {} lines", skip_lines);
    spdlog::debug("CSVParser: (std::istream &is, ...) constructor");
}


template<typename CharT, typename Traits, typename ... Args>
void BasicCSVParser<CharT, Traits, Args...>::SkipLines(int n) {
    std::tuple<Args ...> dummy;
    for (int i = 0; static_cast<bool>(*this) && i < n; ++i) {
        *this >> dummy;
    }
}

template<typename CharT, typename Traits, typename ... Args>
std::vector<std::basic_string<CharT, Traits>> BasicCSVParser<CharT, Traits, Args...>::GetRecordAndParse() {
    using String = std::basic_string<CharT, Traits>;

    String record_str{}; // TODO
    size_t index = 0;
    std::vector<String> record;

    CharT c;
    bool in_quote = false;
    bool is_start = false;
    int field_length = 0;
    while (buff_ >> c) {
        is_start = true;
        if (c == cfg_.line_delimiter) {line_++;}
        if (c == cfg_.quote) {
            in_quote = !in_quote;
            continue;
        }
        if (!in_quote && c == cfg_.delimiter) {

            record.emplace_back(record_str.begin() + index - field_length, record_str.begin() + index);
            field_length = 0;
            continue;
        }
        if (!in_quote && c == cfg_.line_delimiter) {
            break;
        }
        field_length++;
        index++;
        record_str.push_back(c);

    }
    if (!buff_ && !is_start) {
        is_good_ = false;
        return record;
    }

    if (field_length == 0) {
        spdlog::debug("got empty field");
    }
    record.emplace_back(record_str.begin() + index - field_length, record_str.end());


    if (record.size() < sizeof...(Args)) {
        std::string tmp {record_str.begin(), record_str.end()};
        throw ParseException(tmp, line_, index + 3, "Expected more fields");
    }
    if (record.size() > sizeof...(Args)) {
        std::string tmp {record_str.begin(), record_str.end()};
        throw ParseException(tmp, line_, index + 3, "Got unexpected extra fields");
    }

    return record;
}


template<typename CharT, typename Traits, typename ... Args>
class BasicCSVParser<CharT, Traits, Args...>::InputIterator {
public:

    using iterator_category = std::input_iterator_tag;
    using value_type = std::tuple<Args...>;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type *;
    using reference = value_type &;

    InputIterator() = default;
    explicit InputIterator(BasicCSVParser *parser) : parser_(parser) {
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
    BasicCSVParser *parser_;
    std::tuple<Args...> value_;
};



#endif //CSV_PARSER_H


