#ifndef CONVERTER_FACTORY_H
#define CONVERTER_FACTORY_H

#include <functional>
#include <string>
#include <utility>
#include "../converters/i_converter.h"

namespace converter {
    inline void print_strings(std::ostream &os,  const std::string &header, std::vector<std::string> &examples) {
        os << header;
        if (!examples.empty()) {
            os << examples[0];
        }
        for (int i = 1; i < examples.size(); i++) {
            os << ", " << examples[i] << "";
        }
    }


    class ConverterFactory {
    public:
        using Creator = std::shared_ptr<IConverterCreator>;

        // to register converter, call RegisterConverter in constructor
        ConverterFactory() = default;


        // add converter to factory in runtime.
        static void RegisterConverter(const std::string &name, Creator creator) {
            creators[name] = std::move(creator);
        }

        static std::unique_ptr<IConverter> CreateConverter(std::string const &name, size_t line, Params const &params) {
            if (const auto it = creators.find(name); it == creators.cend()) {
                throw std::invalid_argument("Unknown converter: \"" + name + "\" at line " + std::to_string(line));
            }
            auto res = creators[name]->Create(params);
            return res;
        }

        static void PrintHelp(std::ostream & os) {
            os << "Converters: \n";
            for (const auto &[key, creator] : creators) {
                HelpDescriptor desc = creator->GetHelpDescriptor();

                os << "\t" << creator->GetName() << "\n";
                os << "\t\t" "description:" "\t" << desc.description << "\n";
                print_strings(os, "\t\t" "params:" "\t\t\t", desc.parameters);
                os << "\n";
                print_strings(os, "\t\t" "examples:" "\t\t", desc.examples);
                os << "\n";
            }
        }

    private:
        static inline std::unordered_map<std::string /*name*/, Creator /* constructor func */> creators;
    };


    template<typename _=void>
    struct AutoRegisterConverter {
        AutoRegisterConverter(const std::string &name, std::shared_ptr<IConverterCreator> creator) {
            ConverterFactory::RegisterConverter(name, std::move(creator));
        }
    };
}
#endif

