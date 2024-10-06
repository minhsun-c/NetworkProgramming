#ifndef __ENVIRONMENTAL_COMMAND_H
#define __ENVIRONMENTAL_COMMAND_H

void myprintenv();

void mysetenv(char *param);
void free_env();
char **find_env_name(char *name);
bool read_env();

#endif