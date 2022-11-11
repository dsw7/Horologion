#pragma once

std::string epoch_time_to_ascii_time(const std::time_t &epoch_time);
std::time_t get_epoch_time_from_configs(const int &hour, const int &minute, const int &second);
