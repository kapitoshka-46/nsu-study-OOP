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
        ConverterFactory(const std::vector<std::string> &input_files);    // Здесь будут регистрироваться все класы


        // add converter to factory in runtime.
        void RegisterConverter(const std::string &name, Creator creator, HelpDescriptor help_desc) {
            creators[name] = std::move(creator);
            converters_descriptors.push_back(help_desc);
        }

    private:
        std::unordered_map<std::string /*name*/, Creator> creators;
        std::vector <HelpDescriptor> converters_descriptors;
        std::vector<std::string> input_files;
    };
}
#endif

