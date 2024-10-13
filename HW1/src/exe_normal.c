#include "includes.h"

static int child_command(command_t *command)
{
    char *arg[16];
    switch (command->bin_command)
    {
    case IS_BIN_COMMAND:
        arg[0] = command->data.full_path;
        for (int i = 0; i < command->data.param_count; i++)
            arg[i + 1] = command->data.parameter[i];
        arg[command->data.param_count + 1] = NULL;
        if (execvp(arg[0], arg) < 0)
        {
            perror("EXECV Error");
            return -1;
        }
        break;
    case IS_BUILTIN_COMMAND:
        if (command->data.param_count == 0)
            command->data.fptr(NULL);
        else
            command->data.fptr(command->data.parameter[0]);
        break;
    default:
        break;
    }
    exit(EXIT_SUCCESS);
    return 1;
}

static int parent_command(command_t *command)
{
    pid_t pid = fork();
    switch (pid)
    {
    case -1:
        perror("Fork Error");
        return -1;
    case 0:
        child_command(command);
        break;
    default:
        waitpid(pid, NULL, 0);
        break;
    }
}

int single_command_handler(command_t *command)
{
    parent_command(command);
}