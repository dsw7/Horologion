#include "utils_file.h"

bool file_exists(std::string &filepath)
{
    struct stat info;

    if (stat(filepath.c_str(), &info) != 0)
    {
        Logger::error(strerror(errno));
        return false;
    }

    return true;
}

void write_to_file(std::string &filepath, std::string &message)
{
    Logger::info("Will write " + message + " to " + filepath);

    std::ofstream file;

    file.open(filepath);
    file << message;
    file.close();
}
