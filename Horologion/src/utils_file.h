#ifndef UTILS_FILE_H
#define UTILS_FILE_H

#include <string>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#include "logger.h"

bool file_exists(const std::string &filepath);
bool write_to_file(const std::string &filepath, std::string &message);
void overwrite_file(const std::string &filepath, std::string &message);
bool read_file(const std::string &filepath, std::string &file_contents);
bool remove_file(const std::string &filepath);

#endif
