#include "lexer.h"
#include "../terminal/color.h"

#include <iostream>
#include <sstream>



namespace cfg {
    std::string repeat_char(int num, char c) {
        std::string spaced;
        spaced.reserve(num);
        for (auto i = 0; i < num; i++) {
            spaced.push_back(c);
        }
        return spaced;
    }

    void Lexer::SkipSpaces() {

    }

    std::string remove_spaces(const std::smatch::string_type & str) {
        auto beg = str.find_first_not_of(" ");
        auto end = str.find_last_not_of(" ");
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
            SkipSpaces();
            std::regex re(R"(^\s*)" + RegexpForToken(token_kind));
            std::smatch match;
            if (std::regex_search(line.cbegin() + pos_.x, line.cend(), match, re)) {
                pos_.x += match.position();
                //std::cout << "match expr at " << pos_ << ": " << match.str() << '\n';
                pos_.x += match.length();
                if (token_kind != kComment) {    // don't need comments and NewLines
                    tokens_.emplace_back(token_kind, remove_spaces(match.str()), pos_);
                }
                return true;
            }
        }
        std::ostringstream oss;
        oss << "Invalid expression at line " << pos_.y + 1 << ", symbol " << pos_.x + 1 << "\n";
        oss << line.substr(0, pos_.x) << line.substr(pos_.x) << "\n";
        oss << repeat_char(pos_.x + 1, ' ') << "^"  << "\n";

        throw std::invalid_argument(oss.str());
    }

    std::vector<Token> Lexer::GetTokensList() {
        while (Next()) {}   // if already processed file returns false
        return tokens_;
    }

    Lexer::Lexer(const std::string &filename): input(filename) {
        std::getline(input, line);
    }
}
