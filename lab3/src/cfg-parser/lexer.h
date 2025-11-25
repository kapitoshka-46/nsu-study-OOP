#ifndef LEXER_H
#define LEXER_H
#include <fstream>
#include <string>
#include <vector>

#include "point.h"
#include "token.h"


namespace cfg {

    class Lexer {
        std::ifstream input;
        std::string line;
        Point<size_t> pos_ = {0, 0};
        std::vector<Token> tokens_;

        void SkipSpaces();

        bool Next();

    public:
        std::vector<Token> GetTokensList();
        explicit Lexer(std::string const &filename);
    };
}



#endif //LEXER_H
