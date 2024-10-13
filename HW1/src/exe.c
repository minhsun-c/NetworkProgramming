#include <stdio.h>
#include <stdlib.h>

#include "includes.h"

extern env_t *env;

int match_function(command_t *command)
{
    while (command)
    {
        if (command->data.name[0] == 0)
        {
            command->bin_command = IS_INVALID_COMMAND;
            return 0;
        }
        builtin_command_t *check_builtin = find_builtin_command(command->data.name);
        if (check_builtin)
        {
            command->data.fptr = check_builtin->fptr;
            command->bin_command = IS_BUILTIN_COMMAND;
        }
        else
        {
            check_command(command->data.name, command->data.full_path);
            if (command->data.full_path[0] == 0)
            {
                printf("Invalid Command: [%s]\n", command->data.name);
                command->bin_command = IS_INVALID_COMMAND;
                return 0;
            }
            else
            {
                command->bin_command = IS_BIN_COMMAND;
            }
        }
        command = command->next;
    }
    return 1;
}
