#include "TerminalEmulator.h"

#include <string>
#include <iostream>

using std::string;
using enum Command;

const char* ToString(Command co) {
    switch (co){
        case kDump: return "Dump";
        case kHelp: return "Help";
        case kTick: return "Tick";
        case kNone: return "None";
        default: return "???";
    }
}



void TerminalEmulator::Repeat() {
    out << ToString(GetCommand()) << std::endl;
}

Command TerminalEmulator::GetCommand() {
    std::cout << "> ";

    string line;
    std::getline(in, line);

    if (line == "dump")                 return kDump;
    if (line == "help" or line == "?")  return kHelp;
    if (line.substr(0, 5) == "tick " or line.substr(0, 2) == "t ") {
        return kTick;
        // TODO:      store ticks!
    }
    return kNone;
}
