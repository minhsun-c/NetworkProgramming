#include <stdio.h>
#include "includes.h"

#define __TEST_COMMAND
env_t *env;

#ifdef __TEST_ENV

char *title[] = {"RR", "QQ", "PP"};
char *str[] = {"hh", "aa", "uu"};

int main()
{
    char str[][16] = {
        "aa=ll", "kk=004", "uui=eed", "uui=eed"};
    env = mysetenv(env, str[2]);
    env = mysetenv(env, str[3]);
    myprintenv(env, NULL);
    myprintenv(env, "kk");
    myprintenv(env, "aa");
}
#endif // __TEST_ENV

#ifdef __TEST_COMMAND
int main()
{
    char input[128] = "cat Makefile";
    env = construct_env();
    command_t *cmd = parse_input(input);
    match_function(cmd);
}
#endif // __TEST_COMMAND