#ifndef CSV_PARSER_H
#define CSV_PARSER_H
#include <fstream>
#include <memory>
#include <string>
#include <sstream>
#include <tuple>

template <typename ...Args>
class CSVParser {
public:
    explicit CSVParser(const std::string &file, int skip=0);

    explicit CSVParser(std::ifstream &&file, int skip=0);

    explicit CSVParser(std::ifstream &file, int skip=0);

    CSVParser(const CSVParser &other) = delete;
    CSVParser &operator=(const CSVParser &other) = delete;

    CSVParser(const CSVParser &&other)  noexcept = default;
    CSVParser &operator=(CSVParser &&other) = default;

    bool Next();
    const std::tuple<Args ...> &Value() const;

private:
    std::unique_ptr<std::ifstream> owned_file_ {nullptr};
    std::istream &stream_;
    std::tuple<Args...> value_;
};


// ---------------- Constructors --------------------
template<typename ... Args>
CSVParser<Args...>::CSVParser(const std::string &file, int skip)
: owned_file_(std::make_unique<std::ifstream>(file)),
stream_(*owned_file_)
{}

template<typename ... Args>
CSVParser<Args...>::CSVParser(std::ifstream &&file, int skip)
    :owned_file_(std::make_unique<std::ifstream>(std::move(file))),
stream_(*owned_file_)
{}

template<typename ... Args>
CSVParser<Args...>::CSVParser(std::ifstream &file, int skip)
    : stream_(file)
{}

template<typename ... Args>
bool CSVParser<Args...>::Next() {
    constexpr int args_size = sizeof...(Args);

    std::string line;

    std::getline(stream_, line);
    std::replace(line.begin(), line.end(), ',', ' ');
    std::istringstream iss {line};

    if (!stream_) {
        return false;
    }

    iss >> value_;
    return stream_.good();

}

template<typename ... Args>
const std::tuple<Args...> & CSVParser<Args...>::Value() const {
    return value_;
}


#endif //CSV_PARSER_H
