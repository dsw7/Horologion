#pragma once

bool file_exists(const std::string &filepath);
bool write_to_file(const std::string &filepath, const std::string &message);
bool read_file(const std::string &filepath, std::string &file_contents);
