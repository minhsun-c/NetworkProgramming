#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>

#define ERR_MSG_MEM() perror("Memory Allocation Error")

#define MAX_COMMAND_NAME_SIZE 10
#define MAX_COMMAND_PARAM_SIZE 10
#define MAX_COMMAND_PARAM_COUNT 10

#define MAX_ENV_SIZE 10      // maximum number of env variable
#define MAX_ENV_NAME_SIZE 10 // maximum size of env variable name
#define MAX_ENV_DATA_SIZE 32 // maximum size of env variable data

typedef struct command_data
{
    char name[MAX_COMMAND_NAME_SIZE];
    char parameter[MAX_COMMAND_PARAM_COUNT][MAX_COMMAND_PARAM_SIZE];
    int param_count;
} command_data_t;

typedef struct command
{
    command_data_t data;
    int numberpipe;
    struct command *next;
} command_t;

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
int match_function(command_t *command);
env_t *construct_env();