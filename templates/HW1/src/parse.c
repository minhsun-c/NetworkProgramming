#include "includes.h"

/*
    1. Split command_name, parameters, |, |n
    2. Match function or .exe, which is going to be executed
    3. Handle normal pipe
    4. Handle number pipe
    5. Return command_t *command, which stores information above
*/

command_t *parse(char *input)
{
}