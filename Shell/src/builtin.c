#include "includes.h"
#define MAX_PATH_COUNT 20

extern char **myenviron;

/* Record where '=' and ':' is */
static char **getPATH()
{
    char path_name[] = "PATH";
    char **path_val = find_name(path_name);
    char **path_option;
    if ((path_option = (char **)malloc(sizeof(char *) * MAX_PATH_COUNT)) == NULL)
    {
        perror("Memory allocated failed");
        return NULL;
    }
    memset(path_option, 0, sizeof(char *) * MAX_PATH_COUNT);
    char *ptr;
    int cnt = 0;
    for (ptr = *path_val; *ptr; ++ptr)
    {
        if (*ptr == '=' || *ptr == ':')
            path_option[cnt++] = ptr;
    }
    path_option[cnt] = ptr;
    return path_option;
}

void builtin(command_t *cmd)
{
    char **path = getPATH();
    for (char **ptr = path; *ptr; ++ptr)
        printf("%s\n", *ptr);
}