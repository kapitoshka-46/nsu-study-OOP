#ifndef CONVERTER_FACTORY_H
#define CONVERTER_FACTORY_H
#include <functional>
#include <string>
#include "../converters/i_converter.h"

namespace converter {
    struct HelpDescriptor {
        std::string description;
        std::vector<std::string> parameters;
        std::vector<std::string> examples;
    };

    class ConverterFactory {
    public:
        using Creator = std::function<std::unique_ptr<IConverter>(Params)>;

        // to register converter, call RegisterConverter in constructor
        explicit ConverterFactory(const std::vector<std::string> &input_files);


        // add converter to factory in runtime.
        void RegisterConverter(const std::string &name, Creator creator, HelpDescriptor help_desc) {
            creators[name] = std::move(creator);
            converters_helpers.push_back(help_desc);
        }

        std::unique_ptr<IConverter> CreateConverter(std::string const &name, size_t line, Params const &params) {
            //need to find name
            const auto it = creators.find(name);
            if (it == creators.cend()) {
                throw std::invalid_argument("Unknown converter: \"" + name + "\" at line " + std::to_string(line));
            }
            auto res = creators[name](params);
            return res;
        }


    private:
        std::unordered_map<std::string /*name*/, Creator /* constructor func */> creators;
        std::vector <HelpDescriptor> converters_helpers;    // contains info for help message for every converter
        std::vector<int> input_sources;
    };
}
#endif

