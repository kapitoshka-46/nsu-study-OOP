#ifndef MIN_H
#define MIN_H
#include <iostream>

#include "../i_converter.h"


namespace converter {
    class Mix : public IConverter{
    public:
        Mix(std::vector<std::string> others);
        Mix(std::vector<std::string> others, Seconds start);
        Mix(std::vector<std::string> others, Seconds start, Seconds end);


        void Apply() override {
            std::cout << "apply mix\n";
        }
    };

}




#endif //MIN_H
