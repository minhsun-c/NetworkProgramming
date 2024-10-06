#include "includes.h"

void dispatch(command_t *cmd)
{
    char *command = cmd->command;
    char *param = cmd->parameter;
    switch (hash(command))
    {
    case MYPRINTENV:
        myprintenv();
        break;
    case MYSETENV:
        mysetenv(param);
        break;
    default:
        builtin(cmd);
        break;
    }
}