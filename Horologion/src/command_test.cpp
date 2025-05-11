#include "command_test.hpp"

#include "configs.hpp"
#include "logger.hpp"
#include "utils.hpp"
#include "workers.hpp"

#include <string>
#include <thread>
#include <vector>

namespace {

void deploy_jobs(std::vector<std::string> &commands)
{
    logger::info("Testing whether all targets are valid...");

    if (commands.empty()) {
        logger::info("No targets specified. Exiting!");
        return;
    }

    std::vector<std::thread> jobs;

    for (auto &command: commands) {
        jobs.push_back(std::thread(workers::run_command, command));
    }

    for (auto &job: jobs) {
        if (job.joinable()) {
            job.join();
        }
    }

    logger::info("Finished running all targets");
}

} // namespace

namespace commands {

void test()
{
    utils::is_running_as_root();
    Configs configs = read_configs_from_file();
    deploy_jobs(configs.commands);
}

} // namespace commands
