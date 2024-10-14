#ifndef __EXECUTE_PIPED_COMMAND_H
#define __EXECUTE_PIPED_COMMAND_H

#include "types.h"

#define PIPE_READ_END 0
#define PIPE_WRITE_END 1

int pipe_handler(command_t *command);

#endif // __EXECUTE_PIPED_COMMAND_H