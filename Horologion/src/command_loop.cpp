#include "command_loop.hpp"

#include "logger.hpp"
#include "utils.hpp"
#include "workers.hpp"

#include <csignal>
#include <ctime>
#include <string>
#include <thread>

namespace {

const unsigned int SECONDS_PER_DAY = 86400;

void signal_handler(const int signum)
{
    logger::info("Received signal " + std::to_string(signum));
    logger::info("Ending loop");

    utils::unset_rtc_alarm();
    exit(signum);
}

} // namespace

bool CommandLoop::set_times()
{
    this->time_wake = utils::get_epoch_time_from_configs(
        this->configs.time_wake.tm_hour,
        this->configs.time_wake.tm_min,
        this->configs.time_wake.tm_sec // set seconds to zero
    );

    this->time_run_cmd = utils::get_epoch_time_from_configs(
        this->configs.time_run_cmd.tm_hour,
        this->configs.time_run_cmd.tm_min,
        this->configs.time_run_cmd.tm_sec // set seconds to zero
    );

    std::time_t time_sleep = utils::get_epoch_time_from_configs(
        this->configs.time_sleep.tm_hour,
        this->configs.time_sleep.tm_min,
        this->configs.time_sleep.tm_sec // set seconds to zero
    );

    if ((this->time_run_cmd - this->time_wake) < 60) {
        logger::error("The command run time should be at least one minute ahead of the wake time!");
        return false;
    }

    this->wake_duration = time_sleep - this->time_run_cmd;

    if (this->wake_duration < 60) {
        logger::error("The sleep time should be at least one minute ahead of the command run time!");
        return false;
    }

    return true;
}

void CommandLoop::set_alarm()
{
    logger::info("Setting RTC alarm. Next scheduled wake up time:");
    logger::info("The machine will wake up at " + utils::epoch_time_to_ascii_time(this->time_wake));
    logger::info("The machine will wake up at " + std::to_string(this->time_wake) + " seconds since Epoch");

    utils::set_rtc_alarm(this->time_wake);
}

void CommandLoop::deploy_jobs()
{
    unsigned int num_commands = this->configs.commands.size();

    std::vector<std::thread> jobs;
    jobs.push_back(std::thread(worker_stay_awake, &this->wake_duration, &this->configs.suspend_type));

    if (num_commands > 0) {
        for (unsigned int i = 0; i < num_commands; ++i) {
            jobs.push_back(
                std::thread(
                    worker_run_command,
                    &this->configs.commands.at(i).first,
                    &this->configs.commands.at(i).second));
        }
    }

    for (unsigned int i = 0; i < jobs.size(); ++i) {
        jobs.at(i).detach();
    }
}

void CommandLoop::run_loop()
{
    logger::info("Starting loop");

    bool alarm_is_set = false;
    std::time_t current_epoch_time;

    while (true) {
        current_epoch_time = std::time(nullptr);

        if (current_epoch_time > this->time_wake) {
            this->time_wake += SECONDS_PER_DAY;
            alarm_is_set = false;
        }

        if (not alarm_is_set) {
            this->set_alarm();
            alarm_is_set = true;
        }

        if (current_epoch_time == this->time_run_cmd) {
            this->deploy_jobs();
        }

        if (current_epoch_time > this->time_run_cmd) {
            this->time_run_cmd += SECONDS_PER_DAY;
        }

        sleep(1);
    }
}

void CommandLoop::main()
{
    utils::is_running_as_root();

    if (not this->read_configs_from_file()) {
        exit(EXIT_FAILURE);
    }

    if (not this->is_config_file_input_sane()) {
        exit(EXIT_FAILURE);
    }

    if (not this->set_times()) {
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    this->run_loop();
}
