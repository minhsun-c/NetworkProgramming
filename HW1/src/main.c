#include <stdio.h>
#include "includes.h"

#define __TEST_USER_INPUT
env_t *env;
builtin_command_t *builtin;

#ifdef __TEST_ENV

char *title[] = {"RR", "QQ", "PP"};
char *str[] = {"hh", "aa", "uu"};

int main()
{
    char str[][16] = {
        "aa=ll", "kk=004", "uui=eed", "uui=eed"};
    env = construct_env();
    mysetenv(env, str[2]);
    mysetenv(env, str[3]);
    myprintenv(env, NULL);
    myprintenv(env, "kk");
    myprintenv(env, "aa");
}
#endif // __TEST_ENV

#ifdef __TEST_COMMAND
int main()
{
    char input[2][128] = {
        "cat Makefile\n",
        // "python3 test2.py | python3 test.py\n"};
        "cat Makefile | grep obj\n"};
    command_t *cmd;
    init_builtin_command();
    env = construct_env();
    for (int i = 0; i < 2; i++)
    {
        int len = strlen(input[i]);
        input[i][len - 1] = 0;
        printf("%s\n", input[i]);
        cmd = parse_input(input[i]);
        match_function(cmd);
        switch (cmd->type)
        {
        case COMMAND_TYPE_SINGLE:
            single_command_handler(cmd);
            break;
        case COMMAND_TYPE_NORMAL_PIPE:
            pipe_handler(cmd);
            break;
        default:
            break;
        }
        free_command(cmd);
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
    while (1)
    {
        printf("\nunix> ");
        fgets(input, sizeof(input), stdin);
        int len = strlen(input);
        input[len - 1] = 0;
        cmd = parse_input(input);
        if (match_function(cmd) == 0)
        {
            free_command(cmd);
            continue;
        }
        switch (cmd->type)
        {
        case COMMAND_TYPE_SINGLE:
            single_command_handler(cmd);
            break;
        case COMMAND_TYPE_NORMAL_PIPE:
            pipe_handler(cmd);
            break;
        case COMMAND_TYPE_QUIT:
            cmd->data.fptr(NULL);
            free_command(cmd);
            break;
        default:
            free_command(cmd);
            break;
        }
        free_command(cmd);
    }
}
#endif // __TEST_USER_INPUT