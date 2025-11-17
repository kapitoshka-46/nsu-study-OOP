#include <iostream>

#include <bits/this_thread_sleep.h>
#include <chrono>

#include "cfg-parser/lexer.h"

using namespace  std::chrono_literals;


void print_tokens(const std::vector<cfg::Token> &vec) {
    for (auto const &t : vec) {
        std::cout << t << '\n';
    }
}
int main() {
    cfg::Lexer lex("config.cfg");
    while (lex.Next()) {
    //    std::this_thread::sleep_for(300ms);
    }
    print_tokens(lex.GetTokensList());
    return 0;

}