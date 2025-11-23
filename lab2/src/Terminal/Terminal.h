#ifndef TERMINAL_H
#define TERMINAL_H


#include "../core/Core.h"

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
        explicit CommandDump(std::string path);
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
        explicit CommandLoad(std::string path);
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
        explicit CommandSetSpeed(int speed);
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

        [[nodiscard]] Command *ParseCommand(const std::string &line) const;
    public:
        // ---------------- Constructors ----------------
        Terminal(std::istream &in, std::ostream &out);

        // ---------------- Destructors ----------------
        ~Terminal();

        // ---------------- Getters ----------------
        [[nodiscard]] bool IsExit() const;

        [[nodiscard]] core::Universe *GetUniverse() const;

        [[nodiscard]] Command* GetUserCommand() const;    // waits for user prompt

        [[nodiscard]] int GetSpeed() const;

        // ---------------- Setters ----------------
        void SetSpeed(int speed);

        // ---------------- Actions ----------------
        void InitUniverse(int rows = 30, int cols = 40);

        void ExecuteCommand(const std::string &command);

        void Write(const std::string &msg) const;

        void WriteLine(const std::string &msg) const;

        void DisplayUniverse() const;   // show field and additional info about universe

        void Log(const std::string &msg) const;

        void SendExit();

        void RunLoop(); // main loop

        void HelpMessage() const;


        // ----------- ANSI manipulators -----------
        void HideCursor() const;

        void ShowCursor() const;

        void ClearScreen() const;

        void MoveCursorToStart() const;
    };


}


#endif //TERMINAL_H
