#include "includes.h"

static int child_command(command_t *command, int fd[], int RorW)
{
    // printf("get in %s\n", command->data.name);
    if (RorW == PIPE_WRITE_END)
    {
        close(fd[PIPE_READ_END]);
        if (dup2(fd[PIPE_WRITE_END], STDOUT_FILENO) < 0)
        {
            perror("W dup2 Error");
            exit(EXIT_FAILURE);
            return -1;
        }
        close(fd[PIPE_WRITE_END]);
    }
    else if (RorW == PIPE_READ_END)
    {
        close(fd[PIPE_WRITE_END]);
        if (dup2(fd[PIPE_READ_END], STDIN_FILENO) < 0)
        {
            perror("R dup2 Error");
            exit(EXIT_FAILURE);
            return -1;
        }
        close(fd[PIPE_READ_END]);
    }
    char *arg[command->data.param_count + 2];
    switch (command->bin_command)
    {
    case IS_BIN_COMMAND:
        arg[0] = command->data.full_path;
        for (int i = 0; i < command->data.param_count; i++)
            arg[i + 1] = command->data.parameter[i];
        arg[command->data.param_count + 1] = NULL;
        if (execv(arg[0], arg) < 0)
        {
            perror("EXECV Error");
            return -1;
        }
        break;
    case IS_BUILTIN_COMMAND:
        command->data.fptr(command->data.parameter[0]);
        break;
    default:
        break;
    }
    return 1;
}

static int parent_command(command_t *command, int fd[], int RorW)
{
    pid_t pid = fork();
    switch (pid)
    {
    case -1:
        perror("Fork Error");
        return -1;
    case 0:
        if (child_command(command, fd, RorW) == -1)
            return -1;
        break;
    default:
        if (RorW == PIPE_READ_END)
        {
            close(fd[PIPE_READ_END]);
            close(fd[PIPE_WRITE_END]);
        }
        waitpid(pid, NULL, 0);
        break;
    }
    return 1;
}

int pipe_handler(command_t *command)
{
    int fd[2];
    if (pipe(fd) < 0)
    {
        perror("Pipe Error");
        return -1;
    }
    parent_command(command, fd, PIPE_WRITE_END);
    parent_command(command->next, fd, PIPE_READ_END);
}