#ifndef TERMINAL_H
#define TERMINAL_H


#include "Core.h"

namespace terminal {
    class Terminal;


    class Command {
    public:
        virtual ~Command() = default;
        virtual void Execute(Terminal &term) = 0;
    };

    class CommandDump : public Command {
        void Execute(Terminal &term) override;
    };

    class CommandStep :public Command {
        int num_steps_;
    public:
        CommandStep(int num_steps = 1);
        void Execute(Terminal &term) override;
    };

    class CommandInvalid : public Command {
        void Execute(Terminal &term) override;
    };

    class CommandShow : public Command {
        void Execute(Terminal &term) override;
    };

    class CommandHelp : public Command {
        void Execute(Terminal &term) override;
    };

    class CommandLoad : public Command {
        std::string path_;
    public:
        explicit CommandLoad(std::string &path);
        void Execute(Terminal &term) override;
    };

    class CommandEmpty : public Command {
        void Execute(Terminal &term) override;
    };

    class Terminal {
        const bool is_loging = false;
        std::istream &in_;
        std::ostream &out_;
        core::Universe *universe_ { nullptr };
    public:

        Terminal(std::istream &in, std::ostream &out);

        void Write(const std::string &msg);
        void WriteLine(const std::string &msg);
        void Log(const std::string &msg);


        core::Universe *GetUniverse();
        void InitUniverse(int rows, int cols);

        void InteractveMode();
        void SilenceMode();
        Command* GetUserCommand();
        void DisplayUniverse();
    };

}


#endif //TERMINAL_H
