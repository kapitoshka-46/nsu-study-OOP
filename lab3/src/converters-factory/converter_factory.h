#ifndef CONVERTER_FACTORY_H
#define CONVERTER_FACTORY_H

#include "../converters/iconverter.h"
#include <functional>
#include <string>
#include <vector>
#include "../converters/converters.h"   // for Seconds type
#include <unordered_map>

namespace converter {



    class ConverterFactory
    {
    public:
        using Creator = std::function<IConverter* (ConverterParams)>;


        static IConverter *CreateConverter(const std::string &name, ConverterParams params);

        static void RegisterClass(const std::string& name, Creator creator) {
            creators_[name] = std::move(creator);
        }
        //static void RegisterClass(const std::string &name, Creator creator);

        static std::unordered_map<std::string, Creator> creators_;

    private:
    };


}
#endif // CONVERTER_FACTORY_H
