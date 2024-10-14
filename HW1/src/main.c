#include <stdio.h>
#include "includes.h"
#include "numpipe.h"

#define __TEST_USER_INPUT

env_t *env;
builtin_command_t *builtin;

#ifdef __TEST_COMMAND
int main()
{
    char str[4][128] = {
        "ls |2\n",
        "cat Makefile |2\n",
        "number\n",
        "number\n"};
    char input[128];
    command_t *cmd;
    init_builtin_command();
    env = construct_env();
    init_numpipe();
    for (int i = 0; i < 4; i++)
    {
        strncpy(input, str[i], sizeof(str[i]));
        printf("unix> ");
        // fgets(input, sizeof(input), stdin);
        int len = strlen(input);
        input[len - 1] = 0;
        if ((cmd = parse_input(input)) == NULL)
        {
            continue;
        }
        if (exe_shell(cmd) == -1)
            break;
        // sleep(50);
    }
}
#endif // __TEST_COMMAND

#ifdef __TEST_USER_INPUT
int main()
{
    char input[128];
    command_t *cmd;
    init_builtin_command();
    env = construct_env();
    init_numpipe();
    while (1)
    {
        printf("unix> ");
        fgets(input, sizeof(input), stdin);
        int len = strlen(input);
        input[len - 1] = 0;
        if (strlen(input) == 0)
            continue;
        if ((cmd = parse_input(input)) == NULL)
        {
            continue;
        }
        if (exe_shell(cmd) == -1)
            break;
    }
    close_numpipe_buffer();
}
#endif // __TEST_USER_INPUT