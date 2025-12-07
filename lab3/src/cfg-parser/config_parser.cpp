#include "config_parser.h"

#include <iosfwd>
#include <vector>


#include "lexer.h"
#include "token.h"
#include "point.h"
#include "../terminal/color.h"
#include "../converters-factory/converter_factory.h"

using namespace cfg;
using namespace converter;


std::vector<NameLineParams> GetNamesAndParams(const std::string &filename);
void print_str_and_params(std::vector<NameLineParams> x);
void print_tokens(const std::vector<Token> &vec);

std::vector<std::unique_ptr<IConverter>> Parser::GetConvertersFromConfig(const std::string &filename, const std::vector<std::string> &input_files) {

    std::vector<NameLineParams> converters_and_params = GetNamesAndParams(filename);

    PointersToConverters ptrs;
    ConverterFactory factory {};
    for (const auto &[name, line, params] : converters_and_params) {
        ptrs.push_back(std::move(factory.CreateConverter(name, line, params)));
    }

    // debug prints
    print_str_and_params(converters_and_params);
    return ptrs;
}

std::vector<NameLineParams> GetNamesAndParams(const std::string &filename) {
    Lexer lexer {filename};
    auto tokens = lexer.GetTokensList();
    //print_tokens(tokens);

    Params params {};
    size_t line = 0;
    std::string name;


    std::vector<NameLineParams> result{};

    if (tokens.empty()) {
        return result;
    }

    for (const Token &t : tokens) {
        switch (t.kind) {
            case kConverter: {
                if (!name.empty()) {
                    result.emplace_back(name, line + 1, params);
                    name.clear();
                    params.streams.clear();
                    params.time_stamps.clear();
                }
                name = t.text;
                line = t.pos.y;
                break;
            }
            case kVar: {
                params.streams.emplace_back(t.text);
                break;
            }
            case kTimeStamp: {
                Seconds secs = Seconds{std::stoi(t.text)};
                params.time_stamps.push_back(secs);
                break;
            }
            default:
                throw std::invalid_argument("parser: get unknown token: " + TokenName(t.kind));
        }
    }
    result.emplace_back(name, line + 1, params);  // emplacing last parameters;
    return result;

}

void print_str_and_params(std::vector<NameLineParams> x) {
    for (const auto& triple : x) {
        auto [name, pos, params] = triple;
        // std::cout << color::bold << "name: " << color::reset <<  name << "\n\t";
        // std::cout << color::bold << "line : " << color::reset << pos << "\n\t";
        // std::cout << color::bold << "params: " << color::reset;
        // for (const auto& source : params.streams) {
        //     std::cout << source << " ";
        // }
        // for (auto timestamp : params.time_stamps) {
        //     std::cout << timestamp << " ";
        // }
        //std::cout << "\n";
    }
}

void print_tokens(const std::vector<Token> &vec) {
    for (auto const &t : vec) {
        std::cout << t << '\n';
    }
}