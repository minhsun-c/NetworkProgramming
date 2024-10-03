#ifndef __COMMAND_H
#define __COMMAND_H

#include "shell.h"

void builtin(command_t *cmd);

void dispatch(command_t *cmd);

#endif