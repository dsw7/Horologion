#ifndef UTILS_FILE_H
#define UTILS_FILE_H

#include <string>
#include <fstream>

#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#include "logger.h"

bool file_exists(const std::string &filepath);
void write_to_file(const std::string &filepath, std::string &message);
void read_file(const std::string &filepath, std::string &file_contents);

#endif
