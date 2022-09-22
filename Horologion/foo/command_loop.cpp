#include "command_loop.h"

void CommandLoop::run_loop()
{
    Logger::info("Starting loop");
}

bool CommandLoop::main()
{
    if (not this->is_running_as_root())
    {
        return false;
    }

    if (not this->read_configs_from_file())
    {
        return false;
    }

    if (not this->is_config_file_input_sane())
    {
        return false;
    }

    this->run_loop();

    return true;
};
