#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>

#include "types.h"
#include "builtin.h"
#include "exe_env.h"
#include "exe_normal.h"
#include "exe_pipe.h"
#include "exe.h"
#include "helper.h"
#include "numpipe.h"
#include "parse.h"

#define ERR_MSG_MEM()                  \
    perror("Memory Allocation Error"); \
    exit(EXIT_FAILURE)

#define IS_BUILTIN_COMMAND 0
#define IS_BIN_COMMAND 1
#define IS_INVALID_COMMAND -1

#define COMMAND_TYPE_QUIT -1
#define COMMAND_TYPE_SINGLE 0
#define COMMAND_TYPE_NORMAL_PIPE 1
#define COMMAND_TYPE_NUMBER_PIPE 2
