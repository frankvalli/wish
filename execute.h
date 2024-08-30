#ifndef EXECUTE_H
#define EXECUTE_H

#include "wish.h"
#include "command.h"

char **init_path();

bool is_valid(command_t *cmd);

void execute_command(command_t *cmd, char **path);

#endif