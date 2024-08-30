#ifndef EXECUTE_H
#define EXECUTE_H

#include "wish.h"
#include "command.h"

char **init_path();

bool is_valid(command_t *cmd);

void execute_command(command_t *cmd, char ***path);

int execute_builtin(command_t *cmd, char ***path);

void execute_program(command_t *cmd, char **path);

void builtin_exit(command_t *cmd);

void builtin_path(command_t *cmd, char ***path);

void builtin_cd(command_t *cmd);

#endif