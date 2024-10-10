#include "includes.h"

extern env_t *env;
#define __SYSTEM_BIN_WORKING

static char *find_command_in_PATH(char *command_name)
{
    env_node_t *list = env->list[0]->next;
    char cwd[256];
    char *filename;

    if ((filename = (char *)malloc(sizeof(char) * 512)) == NULL)
    {
        ERR_MSG_MEM();
        return NULL;
    }

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Get CWD Fialed");
        return NULL;
    }

    while (list)
    {
#ifdef __CWD_BIN_WORKING
        sprintf(filename,
                "%s/%s/%s", cwd, list->data, command_name);
#endif // __CWD_BIN_WORKING
#ifdef __SYSTEM_BIN_WORKING
        sprintf(filename,
                "bin/%s", command_name);
#endif // __SYSTEM_BIN_WORKING
        list = list->next;
        if (filename[0] != '\0')
            return filename;
    }
    return filename;
}

int bin_handler(command_t *command)
{
    char *full_path = find_command_in_PATH(command->data.name);
    if (full_path == NULL)
    {
        return -1;
    }
    pid_t pid = fork();
    int status;
    char *arg[16];
    switch (pid)
    {
    case -1:
        perror("Fork Error");
        break;
    case 0:
        arg[0] = command->data.name;
        for (int i = 0; i < command->data.param_count; i++)
            arg[i + 1] = command->data.parameter[i];
        arg[command->data.param_count] = NULL;
        execv(full_path, arg);
        break;
    default:
        wait(&status);
        break;
    }
    free(full_path);
    return 1;
}