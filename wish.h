#ifndef WISH_H
#define WISH_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>

extern char error_message[30];

void main_loop(char **buffer, char ***path);

void read_from_terminal();

void read_from_file(FILE *batch);

#endif