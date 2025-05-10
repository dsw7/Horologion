#include "command_loop.hpp"

#include "configs.hpp"
#include "logger.hpp"
#include "utils.hpp"
#include "workers.hpp"

#include <csignal>
#include <ctime>
#include <stdexcept>
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

struct Times {
    std::time_t time_wake = 0;
    std::time_t time_run_cmd = 0;
    std::time_t wake_duration = 0;
};

Times set_times(const Configs &configs)
{
    Times times;

    times.time_wake = utils::get_epoch_time_from_configs(
        configs.time_wake.tm_hour,
        configs.time_wake.tm_min,
        configs.time_wake.tm_sec // set seconds to zero
    );

    times.time_run_cmd = utils::get_epoch_time_from_configs(
        configs.time_run_cmd.tm_hour,
        configs.time_run_cmd.tm_min,
        configs.time_run_cmd.tm_sec // set seconds to zero
    );

    std::time_t time_sleep = utils::get_epoch_time_from_configs(
        configs.time_sleep.tm_hour,
        configs.time_sleep.tm_min,
        configs.time_sleep.tm_sec // set seconds to zero
    );

    if ((times.time_run_cmd - times.time_wake) < 60) {
        throw std::runtime_error("The command run time should be at least one minute ahead of the wake time!");
    }

    times.wake_duration = time_sleep - times.time_run_cmd;

    if (times.wake_duration < 60) {
        throw std::runtime_error("The sleep time should be at least one minute ahead of the command run time!");
    }

    return times;
}

void set_alarm(const std::time_t wake_time)
{
    logger::info("Setting RTC alarm. Next scheduled wake up time:");
    logger::info("The machine will wake up at " + utils::epoch_time_to_ascii_time(wake_time));
    logger::info("The machine will wake up at " + std::to_string(wake_time) + " seconds since Epoch");

    utils::set_rtc_alarm(wake_time);
}

void deploy_jobs(Configs &configs, const std::time_t wake_duration)
{
    unsigned int num_commands = configs.commands.size();

    std::vector<std::thread> jobs;
    jobs.push_back(std::thread(worker_stay_awake, &wake_duration, &configs.suspend_type));

    if (num_commands > 0) {
        for (unsigned int i = 0; i < num_commands; ++i) {
            jobs.push_back(
                std::thread(
                    worker_run_command,
                    &configs.commands.at(i).first,
                    &configs.commands.at(i).second));
        }
    }

    for (unsigned int i = 0; i < jobs.size(); ++i) {
        jobs.at(i).detach();
    }
}

void run_loop(Configs &configs, Times &times)
{
    logger::info("Starting loop");

    bool alarm_is_set = false;
    std::time_t current_epoch_time;

    while (true) {
        current_epoch_time = std::time(nullptr);

        if (current_epoch_time > times.time_wake) {
            times.time_wake += SECONDS_PER_DAY;
            alarm_is_set = false;
        }

        if (not alarm_is_set) {
            set_alarm(times.time_wake);
            alarm_is_set = true;
        }

        if (current_epoch_time == times.time_run_cmd) {
            deploy_jobs(configs, times.wake_duration);
        }

        if (current_epoch_time > times.time_run_cmd) {
            times.time_run_cmd += SECONDS_PER_DAY;
        }

        sleep(1);
    }
}

} // namespace

namespace commands {

void loop()
{
    utils::is_running_as_root();
    Configs configs = read_configs_from_file();

    Times times = set_times(configs);

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    run_loop(configs, times);
}

} // namespace commands
