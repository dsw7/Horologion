#ifndef COMMAND_TRIGGER_H
#define COMMAND_TRIGGER_H

#include <errno.h>
#include <string.h>
#include <linux/reboot.h>  // For LINUX_REBOOT_CMD_POWER_OFF

// System calls
#include <unistd.h>        // For sync(2)
#include <sys/reboot.h>    // For reboot(2)

#include "logger.h"
#include "command_base.h"

class CommandTrigger: public CommandBase
{
    private:
        bool shutdown();

    public:
        bool main();
};

#endif
