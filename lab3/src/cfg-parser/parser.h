#ifndef PARSER_H
#define PARSER_H
#include <utility>
#include <vector>
#include "token.h"
#include "point.h"

namespace cfg {

    class Parser {
        const std::vector<Token> tokens;
        size_t pos = 0;

        bool Require(TokenKind t, int how_many);
    public:
        bool IsValid() const;
        explicit Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}

    };

} // cfg

#endif //PARSER_H
