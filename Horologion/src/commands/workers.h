#pragma once

void worker_stay_awake(const std::time_t *duration, std::string *suspend_type);
void worker_run_command(const std::string *target, std::string *command);
