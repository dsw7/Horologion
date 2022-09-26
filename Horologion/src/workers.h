#ifndef COMMAND_WORKERS_H
#define COMMAND_WORKERS_H

#include <string>
#include <ctime>
#include <chrono>
#include <thread>
#include <stdio.h>

#include "utils_sysfs.h"

void worker_stay_awake(std::time_t *duration, std::string *suspend_type);
void worker_run_command(std::string *target, std::string *command);

#endif
