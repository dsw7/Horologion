#ifndef COMMAND_BASE_H
#define COMMAND_BASE_H

#include <string>
#include <ctime>
#include <map>
#include <time.h>
#include <unistd.h>

#include "logger.h"
#include "parse_config_file.h"
#include "utils_file.h"
#include "utils_time.h"

const std::string SYSFS_WAKEALARM = "/sys/class/rtc/rtc0/wakealarm";
const std::string PROG_CONFIG = "/etc/horolog.ini";
const std::time_t MINIMUM_WAKE_TIME = 90;

struct Configs
{
    struct tm time_alarm = {0};
    struct tm time_sleep = {0};
};

class CommandBase
{
    protected:
        std::time_t time_alarm;
        std::time_t time_sleep;

        bool is_running_as_root();
        bool config_exists();
        void read_configs_from_file();
        bool wakealarm_exists();
        void reset_rtc_alarm();
        void set_time_alarm();
        void set_time_sleep();
        bool sanitize_wake_sleep_cycle();
        void set_rtc_alarm();

    public:
        Configs configs;
        virtual bool main() {};
};

#endif
