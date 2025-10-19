//
// Created by kapiuser on 19.10.25.
//

#ifndef TERMINALEMULATOR_H
#define TERMINALEMULATOR_H

#include "Command.h"
#include <iostream>


class UserInput {
public:
    Command GetCommand();
};

class TerminalEmulator: UserInput {
private:
    std::istream& in  {std::cin};
    std::ostream& out {std::cout};


public:
    void Repeat();
    Command GetCommand();
};



#endif //TERMINALEMULATOR_H
