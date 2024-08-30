#include "parse.h"

command_t *parse_command(char **buffer) {
    command_t *cmd;
    char *cmd_str = get_command(buffer); // get string until next '&' character

    switch (enumerate_characters(cmd_str, '>')) {
        char *arg;
        char *args;
        char *dest;
        case 0: // if there are no '>' characters simply parse the command
            cmd = init_command();
            while ((arg = strsep(&cmd_str, " \t")) != NULL) { // push arguments one by one
                if ((arg[0] == '\0')) continue; // arg is empty after strsep if only character is whitespace or tab
                push_arg(cmd, arg);
            }
            return cmd;
        case 1: // if there is one '>' character parse args and dest separately
            dest = cmd_str;
            args = strsep(&dest, ">"); // now dest points at start of string after '>'
            cmd = init_command();
            while ((arg = strsep(&args, " \t")) != NULL) { // push arguments one by one
                if ((arg[0] == '\0')) continue;
                push_arg(cmd, arg);
            }
            while ((arg = strsep(&dest, " \t")) != NULL) { // push destinations one by one
                if ((arg[0] == '\0')) continue;
                push_dest(cmd, arg);
            }
            if ((num_args(cmd) == 0) | (num_dests(cmd) != 1)) { // if no arguments or not one destination return NULL
                destroy_command(&cmd);
                return NULL;
            }
            return cmd; // return NULL if number of redirections != 1
        default: // return NULL if there's more than one '>' character
            return NULL;
    }
}

char *get_command(char **buffer) {
    return strsep(buffer, "&");
}

int enumerate_characters(char *buffer, char c) {
    int n = 0;
    for (int i = 0; i < strlen(buffer); i++) {
        if (buffer[i] == c) n++;
    }
    return n;
}