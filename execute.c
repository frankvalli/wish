#include "execute.h"
#include "parse.h"

char **init_path() {
    char **path = (char **) malloc(2 * sizeof(char *));
    if (path == NULL) {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }
    path[0] = strdup("/bin"); // initialize path with "/bin" by default
    path[1] = NULL; // add NULL as termination pointer

    return path;
}

bool is_valid(command_t *cmd) {
    if ((cmd == NULL) | (num_dests(cmd) > 1)) return false;
    return true;
}

void execute_command(command_t *cmd, char ***path) {
    if (execute_builtin(cmd, path) == 0) execute_program(cmd, *path);
}

int execute_builtin(command_t *cmd, char ***path) {
    if (strcmp(cmd -> args[0], "exit") == 0) {
        builtin_exit(cmd);
        return 1;
    } else if (strcmp(cmd -> args[0], "path") == 0) {
        builtin_path(cmd, path);
        return 1;
    } else if (strcmp(cmd -> args[0], "cd") == 0) {
        builtin_cd(cmd);
        return 1;
    }
    
    return 0;
}

void execute_program(command_t *cmd, char **path) {
    char *cmd_path;
    int i;
    for (i = 0; path[i] != NULL; i++) { // check if program exists in any of the available paths
        cmd_path = (char *) malloc((strlen(path[i]) + strlen(cmd -> args[0]) + 2) * sizeof(char));
        if (cmd_path == NULL) {
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }
        strcpy(cmd_path, path[i]);
        strcat(cmd_path, "/");
        strcat(cmd_path, cmd -> args[0]);
        if (access(cmd_path, X_OK) == 0) break;
    }
    if (path[i] == NULL) {
        write(STDERR_FILENO, error_message, strlen(error_message));
        return;
    }

    int saved_stdout = dup(STDOUT_FILENO);
    int saved_stderr = dup(STDERR_FILENO);
    if (num_dests(cmd) == 1) {
        freopen(cmd -> dest[0], "w", stdout);
        freopen(cmd -> dest[0], "w", stderr);
    }
    int rc = fork();
    if (rc < 0) {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    } else if (rc == 0) {
        execv(cmd_path, cmd -> args);
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(0);
    }
    if (num_dests(cmd) == 1) {
        dup2(saved_stdout, STDOUT_FILENO);
        dup2(saved_stderr, STDERR_FILENO);
    }
}

void builtin_exit(command_t *cmd) {
    if (num_args(cmd) > 1) {
        write(STDERR_FILENO, error_message, strlen(error_message));
    } 
    else {
        while(wait(NULL) > 0); // wait for all child processes to finish
        exit(0);
    }
}

void builtin_path(command_t *cmd, char ***path) {
    char *ppath = (*path)[0];
    for (int i = 1; ppath != NULL; i++) {
        free(ppath);
        (*path)[i - 1] = NULL;
        ppath = (*path)[i];
    }
    free(*path);
    
    char **new_path = (char **) malloc(num_args(cmd) * sizeof(char *));
    if (new_path == NULL) {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }
    for (int i = 0; i < num_args(cmd) - 1; i++) {
        if (enumerate_characters(cmd -> args[i], '/') == 0) {
            char *buffer = (char *) malloc(256 * sizeof(char));
            if (buffer == NULL) {
                write(STDERR_FILENO, error_message, strlen(error_message));
                exit(1);
            }
            getcwd(buffer, 256 * sizeof(char));
            strcat(buffer, "/");
            strcat(buffer, cmd -> args[i]);
            new_path[i] = strdup(buffer);
            free(buffer);
        } else new_path[i] = strdup(cmd -> args[i + 1]);
    }
    new_path[num_args(cmd) - 1] = NULL;
    *path = new_path;
}

void builtin_cd(command_t *cmd) {
    switch (num_args(cmd)) {
        case 1:
            write(STDERR_FILENO, error_message, strlen(error_message));
            break;
        case 2:
            if (chdir(cmd -> args[1])) {
                write(STDERR_FILENO, error_message, strlen(error_message));
            }
            break;
        default:
            write(STDERR_FILENO, error_message, strlen(error_message));
            break;
    }
}