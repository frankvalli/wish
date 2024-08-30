#include "parse.h"

command_t *parse_command(char **buffer) {
    command_t *cmd = init_command();
    char *cmd_str = _get_command(buffer); // get string until next '&' character

    switch (_enumerate_characters(cmd_str, '>')) {
        char *arg;
        char *args;
        char *dest;
        case 0: // if there are no '>' characters simply parse the command
            while ((arg = strsep(&cmd_str, " \t")) != NULL) { // push arguments one by one
                if ((arg[0] == '\0')) continue; // arg is empty after strsep if only character is whitespace or tab
                push_arg(cmd, arg);
            }
            break;
        case 1: // if there is one '>' character parse args and dest separately
            dest = cmd_str;
            args = strsep(&dest, ">"); // now dest points at start of string after '>'
            cmd = parse_command(&args); // push arguments to command
            while ((arg = strsep(&dest, " \t")) != NULL) { // push destinations one by one
                if ((arg[0] == '\0')) continue;
                push_dest(cmd, arg);
            }
            break;
        default: // do nothing
            break;
    }
    return cmd;
}

char *_get_command(char **buffer) {
    return strsep(buffer, "&");
}

int _enumerate_characters(char *buffer, char c) {
    int n = 0;
    for (int i = 0; i < strlen(buffer); i++) {
        if (buffer[i] == c) n++;
    }
    return n;
}