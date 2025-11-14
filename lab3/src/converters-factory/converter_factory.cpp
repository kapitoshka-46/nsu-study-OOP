#include "converter_factory.h"

#include <utility>


void converter::ConverterFactory::RegisterClass(const std::string &name, Creator creator) {
    creators_[name] = std::move(creator);
}
