#ifndef DUMMY_H
#define DUMMY_H
#include "../i_converter.h"


namespace converter {

    // Do nothing. Just skip samples
    class Dummy : public IConverter{
    public:
        Dummy() = default;
        void Apply(IAudioIn &input, IAudioOut &output) override;

        [[nodiscard]] std::string GetName() const override {return "dummy";}

        [[nodiscard]] HelpDescriptor GetHelpDescriptor() const override;

    };

}



#endif //DUMMY_H
