#ifndef PARSE_H
#define PARSE_H

#include "wish.h"
#include "command.h"

command_t *parse_command(char **buffer);

#endif