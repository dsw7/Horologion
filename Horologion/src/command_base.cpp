#include "command_base.hpp"

#include "logger.hpp"
#include "utils.hpp"

#include <ctime>
#include <map>
#include <utility>

bool CommandBase::is_config_file_input_sane()
{
    if (this->configs.time_wake.tm_hour < 0 or this->configs.time_wake.tm_hour > 23) {
        logger::error("Invalid input for \"time-wake-hour\" field. Input must be between [0, 23] hours");
        return false;
    }

    if (this->configs.time_wake.tm_min < 0 or this->configs.time_wake.tm_min > 59) {
        logger::error("Invalid input for \"time-wake-minute\" field. Input must be between [0, 59] minutes");
        return false;
    }

    if (this->configs.time_run_cmd.tm_hour < 0 or this->configs.time_run_cmd.tm_hour > 23) {
        logger::error("Invalid input for \"time-cmd-hour\" field. Input must be between [0, 23] hours");
        return false;
    }

    if (this->configs.time_run_cmd.tm_min < 0 or this->configs.time_run_cmd.tm_min > 59) {
        logger::error("Invalid input for \"time-cmd-minute\" field. Input must be between [0, 59] minutes");
        return false;
    }

    if (this->configs.time_sleep.tm_hour < 0 or this->configs.time_sleep.tm_hour > 23) {
        logger::error("Invalid input for \"time-sleep-hour\" field. Input must be between [0, 23] hours");
        return false;
    }

    if (this->configs.time_sleep.tm_min < 0 or this->configs.time_sleep.tm_min > 59) {
        logger::error("Invalid input for \"time-sleep-minute\" field. Input must be between [0, 59] minutes");
        return false;
    }

    logger::info("Parsed wake up hour (tm_hour): " + std::to_string(this->configs.time_wake.tm_hour));
    logger::info("Parsed wake up minute (tm_min): " + std::to_string(this->configs.time_wake.tm_min));

    logger::info("Parsed command execution hour (tm_hour): " + std::to_string(this->configs.time_run_cmd.tm_hour));
    logger::info("Parsed command execution minute (tm_min): " + std::to_string(this->configs.time_run_cmd.tm_min));

    logger::info("Parsed sleep hour (tm_hour): " + std::to_string(this->configs.time_sleep.tm_hour));
    logger::info("Parsed sleep minute (tm_min): " + std::to_string(this->configs.time_sleep.tm_min));

    return utils::is_valid_suspend_state(this->configs.suspend_type);
}
