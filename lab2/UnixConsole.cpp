#include "UnixConsole.h"
#include "Command.h"

#include <string>
#include <iostream>

using std::string;
using std::getline;



Console::~Console() = default;

int UnixConsole::GetNumOfTicks() {
    return num_ticks;
}

void UnixConsole::Repeat() {
    ICommand* cmd = GetCommand();
    cmd->Execute();
}

ICommand *UnixConsole::GetCommand() {
    std::cout << "> ";

    string line;
    std::getline(in, line);
    if (line == "dump")                 return new CmdDump();
    if (line == "help" or line == "?")  return new CmdHelp();
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
            return new CmdTick();
        } else {
            std::cout << "Num of ticks should be positive integer with space between word and num\n";
            std::cout << "Exmaple: \"tick 20\" \"tick 30\" \"t 5\"" << std::endl;
            return new CmdNone();
        }
    }
    
    return new CmdNone();
}



void UnixConsole::ClearAndDisplay(/* Universe& */) {
    system("clear");
}
