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

const std::string SYSFS_WAKEALARM = "/sys/class/rtc/rtc0/wakealarm";
const unsigned int SECONDS_PER_DAY = 86400;

void set_rtc_alarm(const std::time_t wake_time)
{
    logger::info("Setting RTC alarm. Next scheduled wake up time:");
    logger::info("The machine will wake up at " + utils::epoch_time_to_ascii_time(wake_time));
    logger::info("The machine will wake up at " + std::to_string(wake_time) + " seconds since Epoch");
    utils::write_to_file(SYSFS_WAKEALARM, std::to_string(wake_time));
}

void unset_rtc_alarm()
{
    logger::info("Unsetting RTC alarm");
    utils::write_to_file(SYSFS_WAKEALARM, "0");
}

void signal_handler(const int signum)
{
    logger::info("Received signal " + std::to_string(signum));
    logger::info("Ending loop");

    unset_rtc_alarm();
    exit(signum);
}

std::time_t get_wake_duration(const Configs &configs)
{
    return configs.time_sleep_e - configs.time_cmd_e;
}

void deploy_jobs(Configs &configs, const std::time_t wake_duration)
{
    unsigned int n = configs.commands.size();

    std::vector<std::thread> jobs;
    jobs.push_back(std::thread(workers::stay_awake, wake_duration, configs.suspend_type));

    if (n > 0) {
        for (unsigned int i = 0; i < n; ++i) {
            jobs.push_back(std::thread(workers::run_command, configs.commands[i]));
        }
    }

    for (unsigned int i = 0; i < jobs.size(); ++i) {
        jobs.at(i).detach();
    }
}

void run_loop(Configs &configs)
{
    logger::info("Starting loop");

    bool alarm_is_set = false;
    std::time_t current_epoch_time;

    while (true) {
        current_epoch_time = std::time(nullptr);

        if (current_epoch_time > configs.time_wake_e) {
            configs.time_wake_e += SECONDS_PER_DAY;
            alarm_is_set = false;
        }

        if (not alarm_is_set) {
            set_rtc_alarm(configs.time_wake_e);
            alarm_is_set = true;
        }

        if (current_epoch_time == configs.time_cmd_e) {
            deploy_jobs(configs, get_wake_duration(configs));
        }

        if (current_epoch_time > configs.time_cmd_e) {
            configs.time_cmd_e += SECONDS_PER_DAY;
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

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    run_loop(configs);
}

} // namespace commands
