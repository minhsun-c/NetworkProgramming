#ifndef __DATA_STRUCTURES_TYPE_H
#define __DATA_STRUCTURES_TYPE_H

#define MAX_COMMAND_NAME_SIZE 10
#define MAX_COMMAND_PARAM_SIZE 10
#define MAX_COMMAND_PARAM_COUNT 10
#define MAX_FULL_PATH_SIZE 64

#define MAX_ENV_SIZE 10      // maximum number of env variable
#define MAX_ENV_NAME_SIZE 10 // maximum size of env variable name
#define MAX_ENV_DATA_SIZE 32 // maximum size of env variable data

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

#endif // __DATA_STRUCTURES_TYPE_H