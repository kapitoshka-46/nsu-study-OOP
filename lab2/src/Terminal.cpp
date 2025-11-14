#include "Terminal.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "file_handler.h"
#include <filesystem>
#include <bits/this_thread_sleep.h>

using namespace terminal;
using namespace core;
using namespace std::chrono_literals;

constexpr std::string prompt = "> ";


CommandDump::CommandDump(const std::string & path) : path_(path){}

void CommandDump::Execute(Terminal &term) {
    term.Log("Execute Dump");
    Universe const *u = term.GetUniverse();
    if (!u) {
        term.WriteLine("No universe to save!");
        return;
    }

    std::string filename;
    if (path_.empty()) {
        filename = u->GetName() + "_tick_" +std::to_string(u->GetTickCount()) + ".lif";
    }
    else {
        filename = path_;
    }
    int count = 0;
    std::string new_filename = filename;
    while (std::filesystem::exists(new_filename)) {
        new_filename = filename + '_' + std::to_string(count);
        count++;
    }

    std::ofstream file(new_filename);
    if (!file) {
        term.WriteLine("Error creating file.");
        return;
    }
    file_handler::SaveToFile(file, *u);
    term.WriteLine(std::format("Universe saved to file \"{}\"", new_filename));
    file.close();
}

CommandStep::CommandStep(const int num_steps) : num_steps_(num_steps) {}

void CommandStep::Execute(Terminal &term) {
    term.Log(std::format("Execute Step {} times", num_steps_));

    Universe *u = term.GetUniverse();

    if (!u) {
        term.WriteLine("No Universe! Load it first.");
        return;
    }
    if (num_steps_ < 0) {
        term.WriteLine("Number of ticks should be >= 0");
        return;
    }
    for (int i = 0; i < num_steps_; i++) {
        u->Step();
    }
}

void CommandRandom::Execute(Terminal &term) {
    Universe *u = term.GetUniverse();
    if (!u) {
        term.InitUniverse();
        u = term.GetUniverse();
    }
    u->GenerateRandom();
    term.WriteLine("Generated random universe.");
}

CommandInvalid::CommandInvalid(std::string cmd) :cmd_(cmd){
}

void CommandInvalid::Execute(Terminal &term) {
    term.Log("Execute Invalid");
    term.WriteLine(std::string("Invalid command ") + "\"" + cmd_ + "\"");
    term.WriteLine(R"(Type "help" or "?" to get help)");
}

void CommandShow::Execute(Terminal &term) {
    term.Log("Execute Show");
    term.DisplayUniverse();
}

void CommandHelp::Execute(Terminal &term) {
    term.Log("Execute Help");
    term.WriteLine("Help message!");
}

CommandLoad::CommandLoad(std::string &path) : path_(path) {}

void CommandLoad::Execute(Terminal &term) {
    term.Log("Execute Load");
    Universe* u = term.GetUniverse();
    if (path_.empty()) {
        term.Log("Empty path!");
        term.WriteLine("Path is empty. Try \"load <path>\"");
        return;
    }
    if (!u) {
        term.InitUniverse(30, 40);  // TODO: вынести в какие-нибудь settings
        u = term.GetUniverse();
    }
    std::ifstream in(path_);
    if (!in) {
        term.Log(std::format("Cannot open ifstream for path {}", path_));
        term.WriteLine(std::format("Cannot open file. Check if path \"{}\" is correct", path_));
        return;
    }
    try {
        file_handler::LoadFromFile(in, *u);
    }
    catch (std::logic_error &e) {
        term.Log(std::format("Catch exception: \"{}\"", e.what()));
        term.WriteLine(e.what());
    }
    catch (std::exception &e) {
        term.Log(std::format("Catch exception: \"{}\"", e.what()));
        term.WriteLine("Unexpected error while loading file! Please, retry.");
    }
    in.close();
}

void CommandEmpty::Execute(Terminal &term) {
    term.Log("Execute Empty");
}

void CommandClear::Execute(Terminal &term) {
    term.Write("\033[2J");
    system("clear");
}

void CommandExit::Execute(Terminal &term) {
    term.Exit();
    term.WriteLine("Exit...");
}

CommandSetSpeed::CommandSetSpeed(int speed) : speed_(speed){}

void CommandSetSpeed::Execute(Terminal &term) {
    term.SetSpeed(speed_);
}


CommandLive::CommandLive(int num_ticks) : num_ticks_(num_ticks){};

void CommandLive::Execute(Terminal &term) {
    Universe *u = term.GetUniverse();

    if (!u) {
        term.WriteLine("No Universe! Load it first.");
        return;
    }
    if (num_ticks_ < 0) {
        term.WriteLine("Number of ticks should be >= 0");
        return;
    }
    // TODO: вынести это в функции терминала + добавить поддержку винды
    term.Write("\033[?25l"); // ANSI hide cursor
    term.Write("\033[2J");  // ANSI clear screen
    for (int i = 0; i < num_ticks_; i++) {
        term.Write("\033[H");   // ANSI move cursor to (0,0)
        u->Step();
        term.DisplayUniverse();
        std::this_thread::sleep_for(1000ms / term.GetSpeed());
    }
    term.Write("\033[?25h"); // ANSI show cursor
}

void Terminal::Write(const std::string &msg) {
    out_ << msg;
}

void Terminal::WriteLine(const std::string &msg) {
    out_ << msg << std::endl;
}

void Terminal::Log(const std::string &msg) {
    if (is_loging) {
        //TODO: class Loger if i need more complex log systems
        out_ << "[Log]: " << msg << '\n';
    }
}

bool Terminal::IsExit() const {
    return is_exit;
}

Universe *Terminal::GetUniverse() {
    return universe_;
}

void Terminal::InitUniverse(int rows, int cols) {
    delete universe_;
    universe_ = new Universe(rows, cols);
}


Terminal::Terminal(std::istream &in, std::ostream &out) :
    in_(in),
    out_(out) {
}

Terminal::~Terminal() {
    std::cerr << "term dest\n";
    delete universe_;
}

void Terminal::ExecuteCommand(const std::string &command) {
    Command *cmd = ParseCommand(command);
    cmd->Execute(*this);
    delete cmd;
}

Command *Terminal::ParseCommand(const std::string &line) {
    std::istringstream iss(line);

    std::string param;
    iss >> param;

    if (param == "help" or param == "?") {
        return new CommandHelp();
    }

    if (param  == "exit") {
        return new CommandExit();
    }
    if (param.empty()) {
        return new CommandEmpty();
    }

    if (param == "clear") {
        return new CommandClear();
    }

    if (param == "speed") {
        std::string speed_str;
        iss >> speed_str;
        if  (iss.bad()){
            WriteLine("Unexpected error while reading input. Please try again.");
            Log("Badbit is set!");
            return new CommandEmpty();
        }

        if (speed_str.empty()) {
            WriteLine("Current speed: " + std::to_string(speed_) + " (default=5)");
            return new CommandEmpty();
        }
        int speed;
        try {
            speed = std::stoi(speed_str);
        }
        catch (std::exception &e) {
            Log(std::format("Exception {}",e.what()));
            WriteLine("Speed should be a positive integer number. Try: \"speed <num>\"");
            return new CommandEmpty();
        }
        if (speed <= 0) {
            WriteLine("Speed should be a positive integer number. Try: \"speed <num>\"");\
            return new CommandEmpty();
        }
        return new CommandSetSpeed(speed);
    }

    if (param == "dump") {
        std::string filename;
        iss >> filename;
        return new CommandDump(filename);
    }

    if (param == "random") {
        return new CommandRandom();
    }

    if (param == "tick" or param == "t") {
        std::string num_ticks_str;
        iss >> num_ticks_str;
        if  (iss.bad()){
            WriteLine("Unexpected error while reading input. Please try again.");
            Log("Badbit is set!");
            return new CommandEmpty();
        }

        if (num_ticks_str.empty()) {
            return new CommandStep();
        }

        int num_ticks = 0;

        try {
            num_ticks = std::stoi(num_ticks_str);
        }
        catch (std::exception &e) {
            Log(std::format("Exception {}",e.what()));
            WriteLine("Num of ticks should be a number. Try: \"tick <num>\"");
            return new CommandEmpty();
        }

        return new CommandStep(num_ticks);
    }

    if (param == "live") {
        std::string num_ticks_str;
        iss >> num_ticks_str;

        if (iss.bad()){
            WriteLine("Unexpected error while reading input. Please try again.");
            Log("Badbit is set!");
            return new CommandEmpty();
        }

        if (num_ticks_str.empty()) {
            WriteLine("You need to specify number of ticks that universe should live");
            return new CommandEmpty();
        }

        int num_ticks = 0;

        try {
            num_ticks = std::stoi(num_ticks_str);
        }
        catch (std::exception &e) {
            Log(std::format("Exception {}",e.what()));
            WriteLine("Num of ticks should be a number. Example: \"live 100\"");
            return new CommandEmpty();
        }

        return new CommandLive(num_ticks);
    }

    if (param == "show") {
        return new CommandShow();
    }

    if (param == "load") {
        std::string filename;
        iss >> filename;

        if (filename.empty()) {
            Log("Empty filename.");
            WriteLine("You need to specify the filename.");
            return new CommandEmpty();
        }

        return new CommandLoad(filename);
    }

    return new CommandInvalid(line);
}
Command *Terminal::GetUserCommand() {
    out_ << prompt;
    std::string line;
    std::getline(in_, line);

    return ParseCommand(line);
}

void Terminal::DisplayUniverse() {
    if (!universe_) {
        WriteLine("Empty Universe");
        return;
    }
    out_ << *universe_;
    out_.flush();

}

void Terminal::Exit() {
    is_exit = true;
}

void Terminal::SetSpeed(int speed) {
    if (speed <= 0) {
        throw std::invalid_argument("speed should be > 0");
    }
    speed_ = speed;
}

int Terminal::GetSpeed() const {
    return speed_;
}

void Terminal::RunLoop() {
    while (not IsExit()) {
        Command *cmd  = GetUserCommand();
        cmd->Execute(*this);

        delete cmd;
    }
}
