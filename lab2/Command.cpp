//
// Created by kapiuser on 19.10.25.
//

#include "Command.h"
#include <iostream>

void CmdDump::Execute() {
    std::cerr << "Executed <dump>" << std::endl;;
}

void CmdHelp::Execute() {
    std::cerr << "Execute <help>" << std::endl;
}

void CmdNone::Execute() {
    std::cerr << "None" << std::endl;
}

void CmdTick::Execute() {
    std::cerr << "Execute CmdTick" << std::endl;
}



