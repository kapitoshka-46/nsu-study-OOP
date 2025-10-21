//
// Created by kapiuser on 19.10.25.
//

#ifndef TERMINALEMULATOR_H
#define TERMINALEMULATOR_H

#include "Command.h"
#include <iostream>


class Console {
public:
    virtual ~Console();

    virtual void Repeat() = 0;
    virtual int GetNumOfTicks() = 0;
    virtual Command GetCommand() = 0;
    virtual void ClearAndDisplay() = 0;
};

class UnixConsole : public Console {
private:
    int num_ticks {};
    std::istream& in  {std::cin};
    std::ostream& out {std::cout};


public:

    int GetNumOfTicks() override;
    void Repeat() override;
    Command GetCommand() override;
    void ClearAndDisplay() override;
};



#endif //TERMINALEMULATOR_H
