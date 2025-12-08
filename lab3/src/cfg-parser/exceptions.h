#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <stdexcept>
#include <format>

namespace cfg {
    inline std::string repeat_char(int num, char c) {
        std::string spaced;
        spaced.reserve(num);
        for (auto i = 0; i < num; i++) {
            spaced.push_back(c);
        }
        return spaced;
    }

    struct ConfigError : std::runtime_error {
        explicit ConfigError(const std::string &msg) : runtime_error(msg){}
    };

    struct LexerError : ConfigError {
        explicit LexerError(const std::string &msg) : ConfigError("Config error\n" + msg){}
    };

    struct InvalidExpressionError : LexerError {
        explicit InvalidExpressionError(const std::string line, const Point<size_t> &pos_)
        :LexerError(std::format("Invalid expression at line {}, symbol {}\n"
            "{}\n"
            "{}^\n",
        pos_.y + 1, pos_.x + 1, line, repeat_char(pos_.x + 1, ' '))) {};
    };

}

#endif //EXCEPTIONS_H
