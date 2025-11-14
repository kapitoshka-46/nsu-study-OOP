#ifndef TERMINAL_H
#define TERMINAL_H


#include "Core.h"

namespace terminal {
    class Terminal;


    class Command {
    public:
        virtual ~Command() = default;
        virtual void Execute(Terminal &term) = 0;

        void Execute();
    };

    class CommandDump : public Command {
        std::string const path_;
    public:
        explicit CommandDump(const std::string &path);
        void Execute(Terminal &term) override;
    };

    class CommandStep :public Command {
        int num_steps_;
    public:
        explicit CommandStep(int num_steps = 1);
        void Execute(Terminal &term) override;
    };

    class CommandRandom : public Command {
        void Execute(Terminal &term) override;
    };

    class CommandInvalid : public Command {
        const std::string cmd_;
    public:
        explicit CommandInvalid(std::string cmd);
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

    class CommandClear : public Command {
        void Execute(Terminal &term) override;
    };

    class CommandExit : public Command {
        void Execute(Terminal &term) override;
    };

    class CommandSetSpeed : public Command {
        int speed_;
    public:
        CommandSetSpeed(int speed);
        void Execute(Terminal &term) override;
    };


    class CommandLive : public Command {
        int num_ticks_;
    public:
        explicit CommandLive(int num_ticks);

        void Execute(Terminal &term) override;

    };

    class Terminal {
        bool is_exit = false;
        const bool is_loging = false;
        int speed_ = 5;
        std::istream &in_;
        std::ostream &out_;
        core::Universe *universe_ { nullptr };

        Command *ParseCommand(const std::string &line);
    public:

        Terminal(std::istream &in, std::ostream &out);
        ~Terminal();

        void ExecuteCommand(const std::string &command);
        void Write(const std::string &msg);
        void WriteLine(const std::string &msg);
        void Log(const std::string &msg);

        bool IsExit() const;

        core::Universe *GetUniverse();
        void InitUniverse(int rows = 30, int cols = 40);

        void InteractveMode();
        void SilenceMode();
        Command* GetUserCommand();
        void DisplayUniverse();

        void Exit();

        void SetSpeed(int speed);
        int GetSpeed() const;

        void RunLoop();
    };

}


#endif //TERMINAL_H
