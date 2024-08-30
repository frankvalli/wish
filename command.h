#ifndef COMMAND_H
#define COMMAND_H

#include "wish.h"

typedef struct command_t {
    char **args;
    char **dest; // allow multiple dests and then check if there's more than one
} command_t;

command_t *init_command();

void push_arg(command_t *cmd, char *arg);

void push_dest(command_t *cmd, char *dest);

void destroy_command(command_t **cmd);

#endif