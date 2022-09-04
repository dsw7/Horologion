#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>
#include <fstream>

#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#include "logger.h"

bool file_exists(std::string &filepath);
void write_to_file(std::string &filepath, std::string &message);

#endif
