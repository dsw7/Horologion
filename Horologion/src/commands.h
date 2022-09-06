#ifndef COMMANDS_H
#define COMMANDS_H

class CommandBase
{
    public:
        virtual bool main() {};
};

class CommandSetAlarm: public CommandBase
{
    public:
        bool main();
};

class CommandResetAlarm: public CommandBase
{
    public:
        bool main();
};

#endif
