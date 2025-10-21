#include "UnixConsole.h"

#include <string>
#include <iostream>

using std::string;
using std::getline;
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


Console::~Console() = default;

int UnixConsole::GetNumOfTicks() {
    return num_ticks;
}

void UnixConsole::Repeat() {
    Command co = GetCommand();
    if (co == kTick) {
        out << ToString(co) << ' ' << num_ticks << std::endl;

    } else {
        out << ToString(co) << std::endl;
    }
}

Command UnixConsole::GetCommand() {
    std::cout << "> ";

    string line;
    std::getline(in, line);
    if (line == "dump")                 return kDump;
    if (line == "help" or line == "?")  return kHelp;
    size_t pos_num {0};
    bool is_cmd_tick = false;
    if (line.substr(0, 2) == "t ") {
        pos_num = 2;
        is_cmd_tick = true;
    }
    if (line.substr(0, 5) == "tick ") {
        pos_num = 5;
        is_cmd_tick = true;
    }
    if (is_cmd_tick) {
        size_t pos_in_str = line.find_first_of("0123456789", pos_num);
        if (pos_num == pos_in_str) {
            num_ticks = std::stoi(line.substr(2));
            return kTick;
        } else {
            std::cout << "Num of ticks should be positive integer with space between word and num\n";
            std::cout << "Exmaple: \"tick 20\" \"tick 30\" \"t 5\"" << std::endl;
            return kNone;
        }
    }
    
    return kNone;
}



void UnixConsole::ClearAndDisplay(/* Universe& */) {
    system("clear");
}
