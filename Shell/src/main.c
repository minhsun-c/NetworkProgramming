#include "includes.h"

char **myenviron;

int main()
{
    myenviron = NULL;
    read_env();
    shell_input();
}