#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <vector>
#include <chrono>
#include <regex>

#include "point.h"

namespace cfg {
    using seconds_t = std::chrono::seconds;


#define TOKEN_LIST kComment,    \
    kVar,                       \
    kTimeStamp,                 \
    kConverter,                 \
    kNewLine,

    enum class TokenKind {
        TOKEN_LIST
    };

    using enum TokenKind;
    constexpr std::array kAllTokens {TOKEN_LIST};


    constexpr std::string RegexpForToken(TokenKind t) {
        switch (t) {
            case kComment:   return R"(\#.*$)";
            case kVar:       return R"(\$\d+)";
            case kTimeStamp: return R"(\d+)";
            case kConverter: return R"(\s*([a-z_]+))";
            case kNewLine:   return R"(\n)";
            default: throw std::invalid_argument("RegexpForToken: Unknown token kind");
        }
    }

    inline std::string TokenName(TokenKind t) {
        switch (t) {
            case kComment:   return R"(Comment)";
            case kVar:       return R"(Var)";
            case kTimeStamp: return R"(TimeStamp)";
            case kConverter: return R"(Converter)";
            case kNewLine:   return R"(NewLine)";
            default: throw std::invalid_argument("Unknown token");
        }
    }


    struct Token {
        TokenKind kind;
        std::string text;
        Point<size_t> pos = {0, 0};

        // functions
        Token(TokenKind token, std::string text, Point<size_t> pos = {0, 0});
    };


    std::ostream & operator<<(std::ostream & lhs, TokenKind rhs);

    std::ostream &operator<<(std::ostream &lhs, const Token &rhs);
}



#endif //TOKEN_H
