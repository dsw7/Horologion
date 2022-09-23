#include "command_loop.h"

void signal_handler(int signum)
{
    Logger::info("Received signal " + std::to_string(signum));
    Logger::info("Ending loop");

    unset_rtc_alarm();
    exit(signum);
}

void worker_stay_awake(std::time_t *duration, std::string *suspend_type)
{
    Logger::info_thread_safe("<target_0> Keeping system awake for " + std::to_string(*duration) + " seconds");
    std::this_thread::sleep_for(std::chrono::seconds(*duration));

    Logger::info_thread_safe("<target_0> " + std::to_string(*duration) + " seconds have elapsed");

    if (suspend_type->compare("none") == 0)
    {
        Logger::info_thread_safe("<target_0> Suspend disabled. Doing nothing");
        return;
    }

    Logger::info_thread_safe("<target_0> Suspending system");
    suspend_system(*suspend_type);

    Logger::info_thread_safe("<target_0> Waking system and terminating this thread");
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

bool CommandLoop::set_times()
{
    this->time_wake = get_epoch_time_from_configs(
        this->configs.time_wake.tm_hour,
        this->configs.time_wake.tm_min,
        this->configs.time_wake.tm_sec  // set seconds to zero
    );

    this->time_run_cmd = get_epoch_time_from_configs(
        this->configs.time_run_cmd.tm_hour,
        this->configs.time_run_cmd.tm_min,
        this->configs.time_run_cmd.tm_sec  // set seconds to zero
    );

    std::time_t time_sleep = get_epoch_time_from_configs(
        this->configs.time_sleep.tm_hour,
        this->configs.time_sleep.tm_min,
        this->configs.time_sleep.tm_sec  // set seconds to zero
    );

    if ((this->time_run_cmd - this->time_wake) < 60)
    {
        Logger::error("The command run time should be at least one minute ahead of the wake time!");
        return false;
    }

    this->wake_duration = time_sleep - this->time_run_cmd;

    if (this->wake_duration < 60)
    {
        Logger::error("The sleep time should be at least one minute ahead of the command run time!");
        return false;
    }

    return true;
}

void CommandLoop::set_alarm()
{
    Logger::info("Setting RTC alarm. Next scheduled wake up time:");
    Logger::info("The machine will wake up at " + epoch_time_to_ascii_time(this->time_wake));
    Logger::info("The machine will wake up at " + std::to_string(this->time_wake) + " seconds since Epoch");

    set_rtc_alarm(this->time_wake);
}

void CommandLoop::deploy_jobs()
{
    unsigned int num_commands = this->configs.commands.size();

    std::vector<std::thread> jobs;
    jobs.push_back(std::thread(worker_stay_awake, &this->wake_duration, &this->configs.suspend_type));

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
        jobs.at(i).detach();
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

        if (current_epoch_time > this->time_wake)
        {
            this->time_wake += SECONDS_PER_DAY;
            alarm_is_set = false;
        }

        if (not alarm_is_set)
        {
            this->set_alarm();
            alarm_is_set = true;
        }

        if (current_epoch_time == this->time_run_cmd)
        {
            this->deploy_jobs();
        }

        if (current_epoch_time > this->time_run_cmd)
        {
            this->time_run_cmd += SECONDS_PER_DAY;
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

    if (not this->set_times())
    {
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    this->run_loop();
};
