#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H
#include <vector>


#include "token.h"
#include "../converters/i_converter.h"
#include "../converters-factory/converter_factory.h"
namespace cfg {



    // name: "EFFECT", text: "mute"
    // name: "Var", text: "$0"
    // name: "time", text: "30"
    class ConfigParser {
    public:
        static std::vector<std::unique_ptr<converter::IConverter>> GetConvertersFromConfig(const std::string &filename, const std::vector<std::string> &input_files);
    };

} // cfg

#endif //CONFIG_PARSER_H
