#include "lexer.h"
#include "../terminal/color.h"

#include <iostream>
#include <sstream>

#include "exceptions.h"


namespace cfg {

    void Lexer::SkipSpaces() {

    }

    std::string remove_spaces(const std::smatch::string_type & str) {
        auto beg = str.find_first_not_of(' ');
        auto end = str.find_last_not_of(' ');
        if (beg == std::smatch::string_type::npos) {beg = 0;}
        if (end == std::smatch::string_type::npos) {end = str.length();}
        return str.substr(beg, end + 1);
    }

    bool Lexer::Next() {
        if (pos_.x >= line.length()) {
            if (not std::getline(input, line)) {
                return false;
            }
            //tokens_.emplace_back(kNewLine, "\\n", pos_);
            pos_.y += 1;
            pos_.x = 0;
        }

        if (line.empty()) { return true; }

        for (const auto &token_kind: kAllTokens) {
            std::regex re(R"(^\s*)" + RegexpForToken(token_kind));
            std::smatch match;
            if (std::regex_search(line.cbegin() + pos_.x, line.cend(), match, re)) {
                if (token_kind != kComment) {    // don't need comments and NewLines
                    tokens_.emplace_back(token_kind, remove_spaces(match.str()), pos_);
                }
                if (token_kind == kConverter and pos_.x != 0) {
                    break;  //converter should start from beginning of file
                }

                pos_.x += match.position();
                //std::cout << "match expr at " << pos_ << ": " << match.str() << '\n';
                pos_.x += match.length();

                return true;
            }
            SkipSpaces();
        }
        throw InvalidExpressionError(line, pos_);
    }

    std::vector<Token> Lexer::GetTokensList() {
        while (Next()) {}   // if already processed file returns false
        return tokens_;
    }

    Lexer::Lexer(const std::string &filename): input(filename) {
        std::getline(input, line);
    }
}
