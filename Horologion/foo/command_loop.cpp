#include "command_loop.h"

void CommandLoop::update_window_limits()
{
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
}

void CommandLoop::shift_window_by_one_day()
{
    this->time_wake += SECONDS_PER_DAY;
    this->time_sleep += SECONDS_PER_DAY;
}

void CommandLoop::log_window_limits()
{
    Logger::info("The machine will wake up at " + epoch_time_to_ascii_time(this->time_wake));
    Logger::info("The machine will wake up at " + std::to_string(this->time_wake) + " seconds since Epoch");

    Logger::info("The machine will sleep at " + epoch_time_to_ascii_time(this->time_sleep));
    Logger::info("The machine will sleep at " + std::to_string(this->time_sleep) + " seconds since Epoch");
}

void signal_handler(int signum)
{
    Logger::info("Received signal " + std::to_string(signum));
    Logger::info("Ending loop");

    unset_rtc_alarm();
    exit(signum);
}

void CommandLoop::deploy_jobs()
{
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
        else
        {
            Logger::info("Waiting " + std::to_string(SECONDS_DELAY_WAKE_UP) + " for machine to warm up");
            sleep(SECONDS_DELAY_WAKE_UP);

            this->shift_window_by_one_day();
            this->log_window_limits();

            this->deploy_jobs();
            alarm_is_set = false;
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

    this->update_window_limits();
    this->log_window_limits();

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    this->run_loop();
};
