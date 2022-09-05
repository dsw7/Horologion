#include "utils_time.h"

std::time_t compute_delay(std::time_t offset_seconds)
{
    std::time_t time_since_epoch = std::time(nullptr);

    Logger::info("The current time is: ");
    Logger::info(std::asctime(std::localtime(&time_since_epoch)));

    std::time_t offset_time = time_since_epoch + offset_seconds;

    Logger::info("The offset time was computed as: ");
    Logger::info(std::asctime(std::localtime(&offset_time)));

    return offset_time;
}
