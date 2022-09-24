#include "command_plan.h"

void CommandPlan::print_tree()
{
    std::cout << "Plan:\n";
    std::cout << " " + TL_ELBOW + H_LINE + "{t_w}: " + "\n";
    std::cout << " " + V_LINE + "\n";
    std::cout << " " + V_LINE + "\n";
    std::cout << " " + RIGHT_TEE + H_LINE + "{t_c}: " + "\n";
    std::cout << " " + V_LINE + "\n";
    std::cout << " " + V_LINE + "\n";
    std::cout << " " + BL_ELBOW + H_LINE + "{t_s}: " + "\n";
}

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

    this->print_tree();
};
