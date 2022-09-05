#ifndef COMMAND_LINE_INTERFACE_H
#define COMMAND_LINE_INTERFACE_H

#include <iostream>
#include <getopt.h>

void help_commands(char *filename);
void help_msg_setup_alarm(char *command);
void get_cli_setup_alarm(int argc, char **argv, int &seconds);

#endif
