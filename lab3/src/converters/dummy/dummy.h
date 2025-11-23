#ifndef DUMMY_H
#define DUMMY_H
#include "../i_converter.h"


namespace converter {

    // Do nothing. Just skip samples
    class Dummy : public IConverter{
    public:
        Dummy() = default;
        void Apply(IAudioIn &input, IAudioOut &output) override;

        std::string GetName() const override {return "dummy";}
    };

}



#endif //DUMMY_H
