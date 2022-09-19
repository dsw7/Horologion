#include "utils_file.h"

bool file_exists(const std::string &filepath)
{
    struct stat info;

    if (stat(filepath.c_str(), &info) != 0)
    {
        return false;
    }

    return true;
}

bool write_to_file(const std::string &filepath, std::string &message)
{
    if (not file_exists(filepath))
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

void overwrite_file(const std::string &filepath, std::string &message)
{
    if (not file_exists(filepath))
    {
        Logger::info("File \"" + filepath + "\" does not exist! Creating new file");
    }

    Logger::info("Will write \"" + message + "\" to " + filepath);
    std::ofstream file;

    file.open(filepath);
    file << message;
    file.close();
}

bool read_file(const std::string &filepath, std::string &file_contents)
{
    if (not file_exists(filepath))
    {
        Logger::error("File \"" + filepath + "\" does not exist!");
        return false;
    }

    Logger::info("Reading " + filepath);

    std::ifstream filestream(filepath);

    std::string line;
    while (getline(filestream, line))
    {
        file_contents += line + "\n";
    }

    filestream.close();
    return true;
}

bool remove_file(const std::string &filepath)
{
    if (not file_exists(filepath))
    {
        Logger::error("File \"" + filepath + "\" does not exist!");
        return false;
    }

    Logger::info("Will remove file \"" + filepath + "\"");

    if (remove(filepath.c_str()) != 0)
    {
        Logger::error(strerror(errno));
        return false;
    }

    return true;
}
