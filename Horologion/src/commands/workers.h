#pragma once

void worker_stay_awake(std::time_t *duration, std::string *suspend_type);
void worker_run_command(std::string *target, std::string *command);
