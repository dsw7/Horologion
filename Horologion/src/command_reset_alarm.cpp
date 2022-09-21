#include "command_reset_alarm.h"

void CommandDisable::unset_cron_job()
{
    Logger::info("Unsetting cron job");
    remove_file(ETC_CRONTAB);
}

bool CommandDisable::main()
{
    if (not this->is_running_as_root())
    {
        return false;
    }

    this->unset_cron_job();
    return this->reset_rtc_alarm();
};
