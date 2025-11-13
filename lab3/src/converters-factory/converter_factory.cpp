#include "converter_factory.h"


void converter::ConverterFactory::RegisterClass(const std::string &name, Creator creator) {
    creators_[name] = creator;
}
