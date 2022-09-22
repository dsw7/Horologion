#include "command_loop.h"

// ----------------------------------------------------------------------------------------------------------

void worker_stay_awake(unsigned int *wake_time)
{
    Logger::info_thread_safe("<target_0> Keeping system awake for " + std::to_string(*wake_time) + " seconds");
    std::this_thread::sleep_for(std::chrono::seconds(*wake_time));

    Logger::info_thread_safe("<target_0> " + std::to_string(*wake_time) + " seconds have elapsed");
}

void worker_run_command(std::string *target, std::string *command)
{
    std::array<char, 128> buffer;
    std::string subproc_output;

    if (command->find_first_not_of(' ') == std::string::npos)
    {
        Logger::error_thread_safe("<" + *target + "> Found empty command. Doing nothing");
        return;
    }

    if (command->find(" 2>&1") == std::string::npos)
    {
        // stderr will otherwise leak out to terminal
        *command += " 2>&1";
    }

    Logger::info_thread_safe("<" + *target + "> Deploying target. Command: \"" + *command + "\"");

    FILE* pipe = popen(command->c_str(), "r");

    if (!pipe)
    {
        Logger::error_thread_safe("<" + *target + "> Target could not be started");
        return;
    }

    while (fgets(buffer.data(), 128, pipe) != NULL)
    {
        Logger::info_thread_safe("<" + *target + "> Reading output from target");
        subproc_output += buffer.data();
    }

    if (pclose(pipe) != 0)
    {
        Logger::warning_thread_safe("<" + *target + "> Target exited with non-zero exit code");
    }

    if (subproc_output.size() > 0)
    {
        Logger::info_thread_safe("<" + *target + "> Output from target:\n" + subproc_output);
    }
    else
    {
        Logger::info_thread_safe("<" + *target + "> No output from target");
    }
}

// ----------------------------------------------------------------------------------------------------------

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
    std::vector<std::thread> jobs;

    unsigned int wake_time = this->time_sleep - this->time_wake - SECONDS_DELAY_WAKE_UP;
    jobs.push_back(std::thread(worker_stay_awake, &wake_time));

    unsigned int num_commands = this->configs.commands.size();

    if (num_commands > 0)
    {
        for (unsigned int i = 0; i < num_commands; ++i)
        {
            jobs.push_back(
                std::thread(
                    worker_run_command,
                    &this->configs.commands.at(i).first,
                    &this->configs.commands.at(i).second
                )
            );
        }
    }

    for (unsigned int i = 0; i < jobs.size(); ++i)
    {
        jobs.at(i).join();
    }
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
            Logger::info("Waiting " + std::to_string(SECONDS_DELAY_WAKE_UP) + " seconds for machine to warm up");
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
