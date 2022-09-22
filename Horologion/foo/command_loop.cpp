#include "command_loop.h"

void interrupt_signal_handler(int signum)
{
    Logger::info("Received signal " + std::to_string(signum));
    LOOP_ENABLED = false;
}

void CommandLoop::run_loop()
{
    Logger::info("Starting loop");

    while (LOOP_ENABLED)
    {
        std::time_t current_epoch_time = std::time(nullptr);
        Logger::info(std::to_string(current_epoch_time));

        sleep(1);
    }

    Logger::info("Ending loop");
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

    signal(SIGINT, interrupt_signal_handler);

    this->run_loop();

    return true;
};
