#include "token.h"

#include <utility>

#include "../terminal/color.h"

namespace cfg {
    Token::Token(TokenKind token, std::string text, Point<size_t> pos)
    : kind(token), text(std::move(text)), pos(pos) {

    }

    std::ostream & operator<<(std::ostream &lhs, TokenKind rhs) {
        lhs << TokenName(rhs);
        return lhs;
    }

    std::ostream & operator<<(std::ostream &lhs, const Token &rhs) {
        lhs << "  Token { "
            << color::bold << "kind: "      << color::reset << color::yellow << rhs.kind << color::reset
            << color::bold << ", pos: "     << color::reset << color::reset << rhs.pos  << color::reset
            << color::bold << ", text: "    << color::reset << color::green << "`"<< rhs.text << "`" << color::reset
            << " }";
        return lhs;
    }
}

