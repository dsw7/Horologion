#include "command_loop.h"

void signal_handler(int signum)
{
    Logger::info("Received signal " + std::to_string(signum));
    Logger::info("Ending loop");

    exit(signum);
}

void CommandLoop::run_loop()
{
    Logger::info("Starting loop");

    while (true)
    {
        std::time_t current_epoch_time = std::time(nullptr);
        Logger::info(std::to_string(current_epoch_time));

        sleep(1);
    }
}

void CommandLoop::main()
{
    if (not this->is_running_as_root())
    {
        exit(EXIT_FAILURE);
    }

    if (not this->read_configs_from_file())
    {
        exit(EXIT_FAILURE);
    }

    if (not this->is_config_file_input_sane())
    {
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    this->run_loop();
};
