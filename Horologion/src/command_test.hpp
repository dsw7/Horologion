#pragma once

#include "configs.hpp"

class CommandTest {
public:
    void main();

private:
    Configs configs;
    void deploy_jobs();
};
