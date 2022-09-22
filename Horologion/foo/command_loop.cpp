#include "command_loop.h"

void CommandLoop::compute_wake_sleep_window()
{
    Logger::info("Parsed wake up hour (tm_hour): " + std::to_string(this->configs.time_wake.tm_hour));
    Logger::info("Parsed wake up minute (tm_min): " + std::to_string(this->configs.time_wake.tm_min));

    Logger::info("Parsed sleep hour (tm_hour): " + std::to_string(this->configs.time_sleep.tm_hour));
    Logger::info("Parsed sleep minute (tm_min): " + std::to_string(this->configs.time_sleep.tm_min));

    this->time_wake = get_epoch_time_from_configs(
        this->configs.time_wake.tm_hour,
        this->configs.time_wake.tm_min,
        this->configs.time_wake.tm_sec  // set seconds to zero
    );

    this->time_sleep = get_epoch_time_from_configs(
        this->configs.time_sleep.tm_hour,
        this->configs.time_sleep.tm_min,
        this->configs.time_sleep.tm_sec  // set seconds to zero
    );

    if (wake_time_is_earlier_than_current_time(this->time_wake))
    {
        this->time_wake += SECONDS_PER_DAY;
        this->time_sleep += SECONDS_PER_DAY;
    }

    Logger::info(
        "The machine will wake up at " + epoch_time_to_ascii_time(this->time_wake) + \
        " or " + std::to_string(this->time_wake) + " seconds since Epoch"
    );
    Logger::info(
        "The machine will sleep at " + epoch_time_to_ascii_time(this->time_sleep) + \
        " or " + std::to_string(this->time_sleep) + " seconds since Epoch"
    );
}

void signal_handler(int signum)
{
    Logger::info("Received signal " + std::to_string(signum));
    Logger::info("Ending loop");

    unset_rtc_alarm();
    exit(signum);
}

void CommandLoop::run_loop()
{
    Logger::info("Starting loop");

    bool alarm_is_set = false;
    std::time_t current_epoch_time;

    while (true)
    {
        current_epoch_time = std::time(nullptr);
        Logger::info(std::to_string(current_epoch_time));

        if (current_epoch_time <= this->time_wake)
        {
            if (not alarm_is_set)
            {
                set_rtc_alarm(this->time_wake);
                alarm_is_set = true;
            }
        }

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

    this->compute_wake_sleep_window();

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    this->run_loop();
};
