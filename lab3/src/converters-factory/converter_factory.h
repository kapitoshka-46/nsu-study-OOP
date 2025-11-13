#ifndef CONVERTER_FACTORY_H
#define CONVERTER_FACTORY_H

#include "../converters/iconverter.h"
#include <functional>
#include <string>
#include <unordered_map>

namespace converter {
    class ConverterFactory
    {
    public:
        ConverterFactory();

        using Creator = std::function<IConverter* (/* args */)>;

        void RegisterClass(const std::string &name, Creator creator);

    private:
        std::unordered_map<std::string, Creator> creators_;
    };


}
#endif // CONVERTER_FACTORY_H
