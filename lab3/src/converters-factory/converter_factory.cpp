#include "converter_factory.h"

#include <utility>

//
// void converter::ConverterFactory::RegisterClass(const std::string &name, Creator creator) {
//     creators_[name] = std::move(creator);
// }
using namespace converter;

ConverterParams::ConverterParams(std::vector<Seconds> time_stamps, std::vector<int> sources)
    : time_stamps(std::move(time_stamps)), sources(std::move(sources))
{}

IConverter * ConverterFactory::CreateConverter(const std::string &name, ConverterParams params) {
    auto it = creators_.find(name);
    if (it == creators_.end())
        return nullptr;
    return it->second(params);
}
