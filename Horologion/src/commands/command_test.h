#pragma once

class CommandTest: public CommandBase
{
    public:
        void main();

    private:
        void deploy_jobs();
};
