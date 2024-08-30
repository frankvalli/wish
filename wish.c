#include "wish.h"
#include "command.h"
#include "parse.h"
#include "execute.h"

char prompt[7] = "wish> ";
char error_message[30] = "An error has occurred\n";

int main(int argc, char *argv[]) {
    switch (argc) {
        case 1:
            read_from_terminal();
            break;
        case 2:
            FILE *batch = fopen(argv[1], "r");
            if (batch == NULL) { // exit if bad argument
                write(STDERR_FILENO, error_message, strlen(error_message));
                exit(1);
            }
            read_from_file(batch);
            fclose(batch);
            break; // close wish after reading from file
        case 3:
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
            break;
    }
    return 0;
}

void main_loop(char **buffer, char ***path) {
    command_t *cmd;
    char *tmp = *buffer; // save buffer to free it later
    while (*buffer != NULL) { // repeat until entire line has been read
        cmd = parse_command(buffer); // parse until next '&' character
        if (!is_valid(cmd)) {
            write(STDERR_FILENO, error_message, strlen(error_message));
            continue;
        } else if (num_args(cmd) != 0) {
            execute_command(cmd, path); // execute command
        }
        destroy_command(&cmd); // destroy command
    }
    while(wait(NULL) > 0); // wait until all sub processes are done
    free(tmp); // free buffer once it has been fully read
}

void read_from_terminal() {
    char *buffer = NULL; // set pointer to NULL to make sure getline allocates memory
    char **path = init_path();
    size_t size;

    while (1) {
        write(STDOUT_FILENO, prompt, strlen(prompt)); // write "wish> " to terminal
        getline(&buffer, &size, stdin); // read line
        buffer[strlen(buffer) - 1] = '\0'; // all commands end with a newline when written in terminal
        main_loop(&buffer, &path);
    }
    return;
}

void read_from_file(FILE *batch) {
    char *buffer = NULL; // set pointer to NULL to make sure getline allocates memory
    char **path = init_path();
    size_t size;

    while (getline(&buffer, &size, batch) >= 0) { // execute all commands in file
        if (buffer[strlen(buffer) - 1] == '\n') { // getline also reads the newline character
            buffer[strlen(buffer) - 1] = '\0';
        }
        main_loop(&buffer, &path);
    }
    return;
}