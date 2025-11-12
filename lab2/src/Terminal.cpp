#include "Terminal.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "file_handler.h"

using namespace terminal;
using namespace core;

constexpr std::string prompt = "> ";


void CommandDump::Execute(Terminal &term) {
    term.Log("Execute Dump");
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

void CommandInvalid::Execute(Terminal &term) {
    term.Log("Execute Invalid");
    term.WriteLine("Invalid command");
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
    std::ifstream in;
    in.open(path_);
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

Command *Terminal::GetUserCommand() {
    out_ << prompt;
    std::string line;
    std::getline(in_, line);

    std::istringstream iss(line);

    iss >> line;
    if (line == "help" or line == "?") {
        return new CommandHelp();
    }
    if (line == "tick" or line == "t") {
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
    if (line == "show") {
        return new CommandShow();
    }
    if (line == "load") {
        std::string filename;
        iss >> filename;
        if (filename.empty()) {
            Log("Empty filename.");
            WriteLine("You need to specify the filename.");
            return new CommandEmpty();
        }
        return new CommandLoad(filename);
    }

    return new CommandInvalid();
}

void Terminal::DisplayUniverse() {
    if (!universe_) {
        WriteLine("Empty Universe");
        return;
    }
    out_ << *universe_;

}
