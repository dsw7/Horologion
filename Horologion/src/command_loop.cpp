#include "command_loop.hpp"

#include "configs.hpp"
#include "logger.hpp"
#include "utils.hpp"
#include "workers.hpp"

#include <atomic>
#include <csignal>
#include <ctime>
#include <fmt/format.h>
#include <stdexcept>
#include <string>
#include <thread>

namespace {

const std::string SYSFS_WAKEALARM = "/sys/class/rtc/rtc0/wakealarm";

void set_rtc_alarm(const std::time_t wake_time)
{
    logger::info("Setting RTC alarm");
    logger::info(fmt::format("The machine will wake up at {}", utils::epoch_time_to_ascii_time(wake_time)));
    utils::write_to_file(SYSFS_WAKEALARM, std::to_string(wake_time));
}

void unset_rtc_alarm()
{
    logger::info("Unsetting RTC alarm");
    utils::write_to_file(SYSFS_WAKEALARM, "0");
}

std::time_t get_wake_duration(const Configs &configs)
{
    return configs.time_sleep_e - configs.time_cmd_e;
}

void deploy_jobs(Configs &configs, const std::time_t wake_duration)
{
    std::vector<std::thread> jobs;
    jobs.push_back(std::thread(workers::stay_awake, wake_duration, configs.suspend_type));

    for (auto &command: configs.commands) {
        jobs.push_back(std::thread(workers::run_command, command));
    }

    for (auto &job: jobs) {
        job.detach();
    }
}

std::atomic<bool> SIG_RECEIVED(false);

void signal_handler(__attribute__((unused)) const int signum)
{
    SIG_RECEIVED.store(true);
}

const int SECONDS_PER_DAY = 86400;

void run_loop(Configs &configs)
{
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    logger::info("Starting loop");

    bool alarm_is_set = false;
    std::time_t current_epoch_time;

    while (not SIG_RECEIVED.load()) {
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

    unset_rtc_alarm();
    logger::info("Ending loop");
}

} // namespace

namespace commands {

void loop()
{
    utils::is_running_as_root();

    logger::enable_file_logging();
    Configs configs = read_configs_from_file();
    run_loop(configs);
    logger::disable_file_logging();
}

} // namespace commands
