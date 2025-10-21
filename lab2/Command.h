//
// Created by kapiuser on 19.10.25.
//

#ifndef COMMAND_H
#define COMMAND_H


class ICommand {
public:
    virtual ~ICommand() = default;

    virtual void Execute() = 0;
};

class CmdNone : public ICommand {
    void Execute() override;
};

class CmdHelp : public ICommand {
    void Execute() override;
};

class CmdTick : public ICommand {
    void Execute() override;
};

class CmdDump : public ICommand {
    void Execute() override;
};





#endif //COMMAND_H
