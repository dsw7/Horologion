#include <vector>
#include <thread>

#include "command_base.h"
#include "logger.h"
#include "workers.h"
#include "command_test.h"

void CommandTest::deploy_jobs()
{
    Logger::info("Testing whether all targets are valid...");

    unsigned int num_commands = this->configs.commands.size();

    if (num_commands < 1)
    {
        Logger::info("No targets specified. Exiting!");
        ::exit(EXIT_SUCCESS);
    }

    std::vector<std::thread> jobs;

    for (unsigned int i = 0; i < num_commands; ++i)
    {
        jobs.push_back(
            std::thread(
                worker_run_command,
                &this->configs.commands.at(i).first,
                &this->configs.commands.at(i).second
            )
        );
    }

    for (unsigned int i = 0; i < jobs.size(); ++i)
    {
        jobs.at(i).join();
    }
}

void CommandTest::main()
{
    if (not this->is_running_as_root())
    {
        ::exit(EXIT_FAILURE);
    }

    if (not this->read_configs_from_file())
    {
        ::exit(EXIT_FAILURE);
    }

    this->deploy_jobs();

    Logger::info("Finished running all targets");
}
