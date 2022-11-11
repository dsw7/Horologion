#include <string>
#include <fstream>
#include <sys/stat.h>

#include "logger.h"
#include "utils_file.h"

bool file_exists(const std::string &filepath)
{
    struct stat info;

    if (::stat(filepath.c_str(), &info) != 0)
    {
        return false;
    }

    return true;
}

bool write_to_file(const std::string &filepath, const std::string &message)
{
    if (not ::file_exists(filepath))
    {
        Logger::error("File \"" + filepath + "\" does not exist!");
        return false;
    }

    Logger::info("Will write \"" + message + "\" to " + filepath);

    std::ofstream file;

    file.open(filepath);
    file << message;
    file.close();

    return true;
}

bool read_file(const std::string &filepath, std::string &file_contents)
{
    if (not ::file_exists(filepath))
    {
        Logger::error("File \"" + filepath + "\" does not exist!");
        return false;
    }

    Logger::info("Reading " + filepath);

    std::ifstream filestream(filepath);

    std::string line;
    while (getline(filestream, line)) // macro defined getline?
    {
        file_contents += line + "\n";
    }

    filestream.close();
    return true;
}
