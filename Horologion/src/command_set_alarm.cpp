#include "command_set_alarm.h"

void CommandSetAlarm::set_cron_job()
{
    Logger::info("Setting up cron job");

    std::string cron_str;

    unsigned int tm_min_with_delay = this->configs.time_alarm.tm_min + DELAY_BETWEEN_WAKE_AND_CRON_TRIG_MIN;
    unsigned int tm_hour_with_delay = this->configs.time_alarm.tm_hour;

    // 59 + DELAY_BETWEEN_WAKE_AND_CRON_TRIG_MIN can exceed 59
    if (tm_min_with_delay >= 60)
    {
        tm_min_with_delay -= 60;
        tm_hour_with_delay += 1;
    }

    cron_str += std::to_string(tm_min_with_delay);
    cron_str += " ";
    cron_str += std::to_string(tm_hour_with_delay);
    cron_str += " * * * root ";

    cron_str += PATH_TO_BINARY + " trigger >> ";
    cron_str += PATH_TO_LOG + " 2>&1";
    cron_str += "\n"; // cron.d will ignore file if newline does not exist

    overwrite_file(ETC_CRONTAB, cron_str);
}

bool CommandSetAlarm::main()
{
    if (not this->is_running_as_root())
    {
        return false;
    }

    if (not this->read_configs_from_file())
    {
        return false;
    }

    if (not this->is_config_file_input_sane())
    {
        return false;
    }

    this->set_time_alarm();
    this->set_time_sleep();

    if (not this->sanitize_wake_sleep_cycle())
    {
        return false;
    }

    this->set_rtc_alarm();
    this->set_cron_job();

    return true;
};
