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
    cmd -> args[0] = NULL;
    cmd -> dest = (char **) malloc(sizeof(char *));
    if (cmd -> dest == NULL) {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);        
    }
    cmd -> dest[0] = NULL;
    return cmd;
}

void push_arg(command_t *cmd, char *arg) {
    int n = num_args(cmd);
    cmd -> args = (char **) realloc(cmd -> args, (n + 2) * sizeof(char *));
    cmd -> args[n + 1] = NULL;
    cmd -> args[n] = strdup(arg);
    return;
}

void push_dest(command_t *cmd, char *dest) {
    int n = num_dests(cmd);
    cmd -> dest = (char **) realloc(cmd -> dest, (n + 2) * sizeof(char *));
    cmd -> dest[n + 1] = NULL;
    cmd -> dest[n] = strdup(dest);
}

void destroy_command(command_t **cmd) {
    if (*cmd == NULL) {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }
    int n_args = num_args(*cmd);
    int n_dests = num_dests(*cmd);
    for (int i = 0; i < n_args; i++) {
        free((*cmd) -> args[i]);
        (*cmd) -> args[i] = NULL;
    }
    for (int i = 0; i < n_dests; i++) {
        free((*cmd) -> dest[i]);
        (*cmd) -> dest[i] = NULL;
    }
    free((*cmd) -> args);
    (*cmd) -> args = NULL;
    free((*cmd) -> dest);
    (*cmd) -> dest = NULL;
    free(*cmd);
    *cmd = NULL;
}

int num_args(command_t *cmd) {
    if (cmd == NULL) return -1;
    char *pargs = cmd -> args[0];
    int n = 0;
    while(pargs != NULL) {
        n++;
        pargs = cmd -> args[n];
    }
    return n;
}

int num_dests(command_t *cmd) {
    if (cmd == NULL) return -1;
    char *pdest = cmd -> dest[0];
    int n = 0;
    while(pdest != NULL) {
        n++;
        pdest = cmd -> dest[n];
    }
    return n;
}