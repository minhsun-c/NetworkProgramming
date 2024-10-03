#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "includes.h"

command_t *parser(char *commandStr)
{
    command_t *cmd;
    if ((cmd = (command_t *)malloc(sizeof(command_t))) == NULL)
        perror("Memory allocate error");
    sscanf(commandStr, "%s", cmd->command);
    int x = strlen(cmd->command) + 1;
    sscanf(commandStr + x, "%[^\n]", cmd->paramater);
    return cmd;
}

void shell_input()
{
    char commandStr[256] = {0};
    command_t *cmd = NULL;
    do
    {
        printf("MyShell%% ");
        fgets(commandStr, sizeof(commandStr), stdin);
        commandStr[strlen(commandStr) - 1] = '\0';
        cmd = parser(commandStr);
        dispatch(cmd);
    } while (strcmp(commandStr, "quit") != 0);
    free_env();
}
