#pragma once

#include "parse_config_file.hpp"

class CommandBase {
public:
    Configs configs;
    virtual void main() {};

protected:
    bool is_config_file_input_sane();
};
