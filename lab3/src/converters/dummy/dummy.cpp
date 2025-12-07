#include "dummy.h"

#include "../../converters-factory/converter_factory.h"

using namespace converter;


static AutoRegisterConverter r {"dummy", std::make_shared<DummyCreator>()};

void Dummy::Apply(audio_stream::Context & input_ctx, IAudioOut &output) {
    auto &input  = input_ctx.GetMainInputStream();
    audio_stream::IntSample sample;
    if (!input) {throw std::invalid_argument("no input");}
    if (!output) {throw std::invalid_argument("no output");}
    while (input >> sample) {
        output << sample;
    }
}

std::unique_ptr<IConverter> DummyCreator::Create(Params params) const {
    if (!params.time_stamps.empty() or !params.streams.empty()) {
        throw std::invalid_argument("dummy converter shouldn't have params");
    }
    return std::make_unique<Dummy>();
}

HelpDescriptor DummyCreator::GetHelpDescriptor() const {
    return HelpDescriptor{"do nothing",{  }, {"dummy"} };

}

std::string DummyCreator::GetName() const {
    return "dummy";
}
