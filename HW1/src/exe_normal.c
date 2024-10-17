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

static int open_write_pipe(command_t *command)
{
    if (command->output_numpipe_id >= 0)
    {
        char filename[128];
        getcwd(filename, sizeof(filename));
        sprintf(filename,
                "%s/tmp/buf%d.txt", filename, command->output_numpipe_id);
        int out_fd = open(filename, O_WRONLY | O_TRUNC);
        if (out_fd == -1)
        {
            perror("Output numpipe error");
            exit(EXIT_FAILURE);
            return -1;
        }
        if (dup2(out_fd, STDOUT_FILENO) == -1)
        {
            perror("dup2 Error");
            exit(EXIT_FAILURE);
            return -1;
        }
        close(out_fd);
    }
    return 1;
}

static int child_command(command_t *command)
{
    if (open_read_pipe(command) == -1)
        return -1;
    if (open_write_pipe(command) == -1)
        return -1;
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
        if (slen2 == 0)
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

static int parent_command(command_t *command)
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
        child_command(command);
        break;
    default:
        waitpid(pid, NULL, 0);
        break;
    }
    return 1;
}

int single_command_handler(command_t *command)
{
    parent_command(command);
}