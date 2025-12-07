#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H
#include <vector>


#include "token.h"
#include "../converters/i_converter.h"
#include "../converters-factory/converter_factory.h"
#include <filesystem>
namespace cfg {


    using NameLineParams = std::tuple<std::string, size_t, converter::Params>;
    using PointersToConverters = std::vector<std::unique_ptr<converter::IConverter>>;

    // name: "EFFECT", text: "mute"
    // name: "Var", text: "$0"
    // name: "time", text: "30"
    class Parser {
    public:
        static PointersToConverters GetConvertersFromConfig(const std::string &filename /*, input_files for checking streams ?*/);
    };

} // cfg

#endif //CONFIG_PARSER_H
