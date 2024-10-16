#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct data
{

} data_t;

typedef struct command
{
    data_t data;
    struct command *next;
} command_t;