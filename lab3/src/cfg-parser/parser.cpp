#include "parser.h"
#include "../converters/converters.h"

namespace cfg {
    bool Parser::Require(TokenKind t, int how_many) {
        if (pos + how_many >= tokens.size()) {
            return false;
        }
        for (int i = 0; ; i++) {

        }
    }

    bool Parser::IsValid() const {
        for (Token const& t: tokens) {
            switch (t.kind) {
                case kNewLine: {
                    continue;
                    break;
                }
                case kConverter: {
                    // check if converter exists
                    std::cout << "xxx";
                }
                default: std::cout <<"default";
            }
        }
        return true;
    }
} // cfg