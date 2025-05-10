#include "command_test.hpp"

#include "configs.hpp"
#include "logger.hpp"
#include "utils.hpp"
#include "workers.hpp"

#include <thread>
#include <vector>

void CommandTest::deploy_jobs()
{
    logger::info("Testing whether all targets are valid...");

    unsigned int num_commands = this->configs.commands.size();

    if (num_commands < 1) {
        logger::info("No targets specified. Exiting!");
        exit(EXIT_SUCCESS);
    }

    std::vector<std::thread> jobs;

    for (unsigned int i = 0; i < num_commands; ++i) {
        jobs.push_back(
            std::thread(
                worker_run_command,
                &this->configs.commands.at(i).first,
                &this->configs.commands.at(i).second));
    }

    for (unsigned int i = 0; i < jobs.size(); ++i) {
        jobs.at(i).join();
    }
}

void CommandTest::main()
{
    utils::is_running_as_root();
    this->configs = read_configs_from_file();

    this->deploy_jobs();

    logger::info("Finished running all targets");
}
