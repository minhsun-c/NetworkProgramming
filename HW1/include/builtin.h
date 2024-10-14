#ifndef __BUILTIN_H
#define __BUILTIN_H

void myhelp(void *param);

void myquit(void *param);

builtin_command_t *find_builtin_command(char *name);

int init_builtin_command();

#endif // __BUILTIN_H