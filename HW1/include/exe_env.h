#ifndef __EXECUTE_ENVIRONMENTAL_COMMAND_H
#define __EXECUTE_ENVIRONMENTAL_COMMAND_H

#include "types.h"

env_t *construct_env();

void myprintenv(void *name);

void mysetenv(void *param);

int check_command(char *name, char *path);

#endif // __EXECUTE_ENVIRONMENTAL_COMMAND_H