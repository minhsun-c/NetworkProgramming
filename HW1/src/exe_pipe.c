#include "includes.h"

static int open_read_pipe(command_t *command)
{
    if (command->input_numpipe_id >= 0)
    {
        char filename[128];
        getcwd(filename, sizeof(filename));
        sprintf(filename,
                "%s/tmp/buf%d.txt", filename, command->input_numpipe_id);
        int in_fd = open(filename, O_RDONLY);
        if (in_fd == -1)
        {
            perror("Input numpipe error");
            exit(EXIT_FAILURE);
            return -1;
        }
        if (dup2(in_fd, STDIN_FILENO) == -1)
        {
            perror("dup2 Error");
            exit(EXIT_FAILURE);
            return -1;
        }
        close(in_fd);
    }
    return 1;
}

static int build_pipe_fd(int fd[], int RorW)
{
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
    return 1;
}

static int child_command(command_t *command, int fd[], int RorW)
{
    if (open_read_pipe(command) == -1)
        return -1;
    if (build_pipe_fd(fd, RorW) == -1)
        return -1;
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
            exit(EXIT_FAILURE);
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

static int env_command(command_t *command)
{
    if (strncmp(command->data.name, "setenv", strlen("setenv")) == 0)
    {
        size_t slen1 = strlen(command->data.parameter[0]);
        size_t slen2 = strlen(command->data.parameter[1]);
        size_t slen = slen1 + slen2 + 2;
        if (command->data.param_count != 2)
        {
            command->data.fptr(NULL);
        }
        else
        {
            char *param = (char *)malloc(sizeof(char) * slen);
            sprintf(param, "%s=%s", command->data.parameter[0], command->data.parameter[1]);
            command->data.fptr(param);
            free(param);
        }
        return 1;
    }
    return 0;
}

static int parent_command(command_t *command, int fd[], int RorW)
{
    if (env_command(command))
        return 1;
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
    return 1;
}