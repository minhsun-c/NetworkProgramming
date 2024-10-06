#ifndef __SHELL_H
#define __SHELL_H

typedef struct commandType
{
    char command[100];
    char parameter[100];
} command_t;

command_t *parser(char *commandStr);

void shell_input();

#endif