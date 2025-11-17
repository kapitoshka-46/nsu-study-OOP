#include "lexer.h"
#include "../terminal/color.h"

#include <iostream>
#include <sstream>



namespace cfg {
    template<typename T>
    std::string repeat_char(T num, char c) {
        std::string spaced;
        spaced.reserve(num);
        for (auto i = 0; i < num; i++) {
            spaced.push_back(c);
        }
        return spaced;
    }

    bool Lexer::Next() {
        if (pos_.x >= line.length()) {
            if (not std::getline(input, line)) {
                return false;
            }
            tokens_.emplace_back(kNewLine, "\\n", pos_);
            //std::cout << "\\n\n";
            pos_.y += 1;
            pos_.x = 0;
        }

        if (line.empty()) { return false; }

        for (const auto &t: all_tokens) {
            std::regex re(R"(^\s*)" + RegexpForToken(t));
            std::smatch match;
            if (std::regex_search(line.cbegin() + pos_.x, line.cend(), match, re)) {
                pos_.x += match.position();
                //std::cout << "match expr at " << pos_ << ": " << match.str() << '\n';
                pos_.x += match.length();
                if (t != kComment) {    // don't need comments at all
                    tokens_.emplace_back(t, match.str(), pos_);
                }
                return true;
            }
        }

        std::cout << color::red << "invalid expression at line " << pos_.y + 1 << ", symbol " << pos_.x + 1 << color::reset << '\n';
        std::cout << line.substr(0, pos_.x) << line.substr(pos_.x) << color::reset << '\n';
        std::cout << repeat_char(pos_.x + 1, ' ')   << color::red << "^" << color::reset <<'\n';
        pos_.x = line.length();
        return true;
    }

    std::vector<Token> Lexer::GetTokensList() {
        return tokens_;
    }

    Lexer::Lexer(std::string const &filename): input(filename) {
        std::getline(input, line);
    }
}
