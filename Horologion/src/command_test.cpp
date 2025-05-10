#include "command_test.hpp"

#include "configs.hpp"
#include "logger.hpp"
#include "utils.hpp"
#include "workers.hpp"

#include <thread>
#include <vector>

namespace {

void deploy_jobs(Configs &configs)
{
    logger::info("Testing whether all targets are valid...");

    unsigned int num_commands = configs.commands.size();

    if (num_commands < 1) {
        logger::info("No targets specified. Exiting!");
        return;
    }

    std::vector<std::thread> jobs;

    for (unsigned int i = 0; i < num_commands; ++i) {
        jobs.push_back(
            std::thread(
                worker_run_command,
                &configs.commands.at(i).first,
                &configs.commands.at(i).second));
    }

    for (unsigned int i = 0; i < jobs.size(); ++i) {
        jobs.at(i).join();
    }

    logger::info("Finished running all targets");
}

} // namespace

namespace commands {

void test()
{
    utils::is_running_as_root();
    Configs configs = read_configs_from_file();
    deploy_jobs(configs);
}

} // namespace commands
