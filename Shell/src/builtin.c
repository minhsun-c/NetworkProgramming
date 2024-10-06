#include "includes.h"
#define MAX_PATH_COUNT 20
#define SINGLE_PATH_LEN 32

// #define ONLY_BIN_IN_PATH

extern char **myenviron;

static char **getPATH()
{
    char path_name[] = "PATH";
    char **ptr;
    if ((ptr = find_env_name(path_name)) == NULL)
    {
        perror("Invalid Path");
        return NULL;
    }
    char *path_val;
    if ((path_val = (char *)malloc(strlen(*ptr) + 1)) == NULL)
    {
        perror("Memory allocated failed");
        return NULL;
    }
    memcpy(path_val, *ptr, strlen(*ptr) + 1);
    char **path_list;
    if ((path_list = (char **)malloc(sizeof(char *) * MAX_PATH_COUNT)) == NULL)
    {
        perror("Memory allocated failed");
        return NULL;
    }
    memset(path_list, 0, sizeof(char *) * MAX_PATH_COUNT);
    int cnt = 0;
    char *path_option;
    path_option = strtok(path_val, "=");
    while ((path_option = strtok(NULL, ":")))
    {
        path_list[cnt] = (char *)malloc(strlen(path_option));
        if (path_list[cnt] == NULL)
        {
            perror("Memory allocated failed");
            return NULL;
        }
        strncpy(path_list[cnt], path_option, strlen(path_option));
        ++cnt;
    }
    free(path_val);
    return path_list;
}

static bool check_file_existence(command_t *cmd)
{
    if (myenviron == NULL)
    {
        read_env();
    }
    char **path = getPATH();
    if (path == NULL)
    {
        perror("Invalid path");
        return false;
    }
    char cwd[64] = "\0";
    if (!cwd[0])
    {
        if (getcwd(cwd, sizeof(cwd)) == NULL)
        {
            perror("Cannot find current working directory");
            return false;
        }
    }

    char full_path[256];
    snprintf(full_path, sizeof(full_path), "%s/%s/%s", cwd, path[0], cmd->command);

    free(path);

    struct stat buffer;

    return (stat(full_path, &buffer) == 0);
}

void builtin(command_t *cmd)
{
    if (check_file_existence(cmd) == false)
    {
        printf("Unknown Command: %s\n", cmd->command);
        return;
    }

    char cwd[128];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Cannot find current working directory");
        return;
    }

    pid_t pid;
    char path[256];
    char *arg[16];

    switch (pid = fork())
    {
    case 0: /* child */
        // snprintf(path, sizeof(path), "/bin/%s", cmd->command);
        snprintf(path, sizeof(path), "%s/bin/%s", cwd, cmd->command);
        arg[0] = cmd->command;
        arg[1] = cmd->parameter;
        arg[2] = NULL;
        printf("path: %s\n", path);
        printf("param: %s\n", arg[1]);
        printf("cmd len: %d\n", strlen(cmd->command));
        if (execv(path, arg) == -1)
        {
            perror("Exec Error"); // Handle exec failure
            exit(EXIT_FAILURE);
        }
        break;
    case -1:
        perror("Fork Error");
        exit(EXIT_FAILURE);
    default:
        wait(NULL);
        break;
    }
}
