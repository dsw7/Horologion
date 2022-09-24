#include "command_plan.h"

void CommandPlan::main()
{
    if (not this->read_configs_from_file())
    {
        exit(EXIT_FAILURE);
    }

    if (not this->is_config_file_input_sane())
    {
        exit(EXIT_FAILURE);
    }
};
