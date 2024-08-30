#ifndef PARSE_H
#define PARSE_H

#include "wish.h"
#include "command.h"

command_t *parse_command(char **buffer);

char *get_command(char **buffer);

int enumerate_characters(char *buffer, char c);

#endif