#include <stdio.h>
#include <stdlib.h>

#include "includes.h"
#define ISNAME(cmd) (strncmp(name, cmd, len) == 0)

extern env_t *env;

int match_function(command_t *command)
{
    char *name = command->data.name;
    int len = strlen(name);
    if (ISNAME("printenv"))
    {
        myprintenv(env, command->data.parameter[0]);
        return 1;
    }
    else if (ISNAME("setenv"))
    {
        env = mysetenv(env, command->data.parameter[0]);
        return (env != NULL);
    }
    else
    {
        if (bin_handler(command) == -1)
        {
            printf(
                "Invalid Command: [%s]\n",
                command->data.name);
            return 0;
        }
        else
            return 1;
    }
}