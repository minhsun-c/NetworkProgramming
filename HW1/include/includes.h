#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>

#include "numpipe.h"
// #define DEBUG_MODE_ON

#define ERR_MSG_MEM()                  \
    perror("Memory Allocation Error"); \
    exit(EXIT_FAILURE)

#define MAX_COMMAND_NAME_SIZE 10
#define MAX_COMMAND_PARAM_SIZE 10
#define MAX_COMMAND_PARAM_COUNT 10
#define MAX_FULL_PATH_SIZE 64

#define MAX_ENV_SIZE 10      // maximum number of env variable
#define MAX_ENV_NAME_SIZE 10 // maximum size of env variable name
#define MAX_ENV_DATA_SIZE 32 // maximum size of env variable data

#define PIPE_READ_END 0
#define PIPE_WRITE_END 1

#define IS_BUILTIN_COMMAND 0
#define IS_BIN_COMMAND 1
#define IS_INVALID_COMMAND -1

#define COMMAND_TYPE_QUIT -1
#define COMMAND_TYPE_SINGLE 0
#define COMMAND_TYPE_NORMAL_PIPE 1
#define COMMAND_TYPE_NUMBER_PIPE 2

typedef struct command_data
{
    char name[MAX_COMMAND_NAME_SIZE];
    char parameter[MAX_COMMAND_PARAM_COUNT][MAX_COMMAND_PARAM_SIZE];
    char full_path[MAX_FULL_PATH_SIZE];
    int param_count;
    void (*fptr)(void *);
} command_data_t;

typedef struct command
{
    command_data_t data;
    int numberpipe;
    int output_numpipe_id;
    int input_numpipe_id;
    int bin_command;
    int type;
    struct command *next;
} command_t;

typedef struct builtin_command
{
    char name[MAX_COMMAND_NAME_SIZE];
    void (*fptr)(void *);
    struct builtin_command *next;
} builtin_command_t;

typedef struct env_node
{
    char data[MAX_ENV_DATA_SIZE];
    struct env_node *next;
} env_node_t;

typedef struct env
{
    char name[MAX_ENV_SIZE][MAX_ENV_NAME_SIZE];
    env_node_t *list[MAX_ENV_SIZE];
    int next_empty;
} env_t;

command_t *parse_input(char *input);
int exe_shell(command_t *command);
env_t *construct_env();
void myprintenv(void *name);
int bin_handler(command_t *command);
void mysetenv(void *param);
int check_command(char *name, char *path);
char *strip_space(char *str);
builtin_command_t *find_builtin_command(char *name);
int init_builtin_command();
int pipe_handler(command_t *command);
int single_command_handler(command_t *command);
void free_command(command_t *cmd);
