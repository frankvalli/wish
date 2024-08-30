#include "command.h"

command_t *init_command() {
    command_t *cmd = malloc(sizeof(command_t));
    if (cmd == NULL) {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }
    cmd -> args = (char **) malloc(sizeof(char *));
    if (cmd -> args == NULL) {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);        
    }
    cmd -> dest = (char **) malloc(sizeof(char *));
    if (cmd -> dest == NULL) {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);        
    }
    return cmd;
}

void push_arg(command_t *cmd, char *arg) {
    cmd -> args = (char **) realloc(cmd -> args, (num_args(cmd) + 1) * sizeof(char *));
    cmd -> args[num_args(cmd) - 1] = strdup(arg);
    cmd -> args[num_args(cmd)] = NULL;
    return;
}

void push_dest(command_t *cmd, char *dest) {
    cmd -> dest = (char **) realloc(cmd -> dest, (num_dests(cmd) + 1) * sizeof(char *));
    cmd -> dest[num_dests(cmd) - 1] = strdup(dest);
    cmd -> dest[num_dests(cmd)] = NULL;
}

void destroy_command(command_t **cmd) {
    for (int i = 0; i < num_args(*cmd); i++) free((*cmd) -> args[i]);
    for (int i = 0; i < num_dests(*cmd); i++) free((*cmd) -> dest[i]);
    free((*cmd) -> args);
    (*cmd) -> args = NULL;
    free((*cmd) -> dest);
    (*cmd) -> dest = NULL;
    free(*cmd);
    *cmd = NULL;
}

int num_args(command_t *cmd) {
    char *pargs = cmd -> args[0];
    int n = 0;
    while(pargs != NULL) {
        n++;
        pargs = cmd -> args[n];
    }
    return n;
}

int num_dests(command_t *cmd) {
    char *pdest = cmd -> dest[0];
    int n = 0;
    while(pdest != NULL) {
        n++;
        pdest = cmd -> dest[n];
    }
    return n;
}