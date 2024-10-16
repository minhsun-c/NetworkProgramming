#include "includes.h"

/* Global Variable
    1. builtin command list, e.g. printenv, setenv, help, quit
    2. environmental variable
 */

int init_shell()
{
    /* 1. builtin command list */
    /* 2. environmental variable */
    /* 3. number pipe */
}

int main()
{
    init_shell();
    char input[128];
    command_t *cmd;
    while (1)
    {
        printf("SHELL% ");
        fgets(input, sizeof(input), stdin);
        int len = strlen(input);
        input[len - 1] = 0;
        if (strlen(input) == 0)
            continue;

        if ((cmd = parse(input)) == NULL)
        {
            continue;
        }
        if (exe_shell(cmd) == -1)
            break;
    }
}