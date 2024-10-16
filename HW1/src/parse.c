#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "includes.h"

static int find_normal_pipe(char *input)
{
    for (int i = 0; input[i]; i++)
    {
        if (input[i] == '|' && input[i + 1] == ' ')
            return i;
    }
    return -1;
}

static int find_number_pipe(char *input)
{
    for (int i = 0; input[i]; i++)
    {
        if (input[i] == '|' && isdigit(input[i + 1]))
            return i;
    }
    return -1;
}

static command_t *split_command_name_param(char *input, int numberpipe)
{
    command_t *command;
    char *temp;
    if ((command = (command_t *)malloc(sizeof(command_t))) == NULL)
    {
        ERR_MSG_MEM();
        return NULL;
    }
    command->next = NULL;
    input = strip_space(input);
    temp = strsep(&input, " ");
    strncpy(command->data.name, temp, strlen(temp));
    for (int i = 0; i < MAX_COMMAND_PARAM_COUNT; i++)
    {
        temp = strsep(&input, " ");
        if (temp == NULL)
            break;
        else if (strlen(temp) == 0)
            i--;
        else
        {
            strncpy(command->data.parameter[i], temp, strlen(temp));
            command->data.param_count++;
        }
    }
    command->input_numpipe_id = do_numpipe();
    command->output_numpipe_id = insert_numpipe_info(numberpipe);
    command->numberpipe = numberpipe;
    if (command->input_numpipe_id >= 0)
    {
        clear_numpipe(command->input_numpipe_id);
    }
    return command;
}

static command_t *split_with_normal_pipe(char *input, int pipeid)
{
    command_t *command_list;
    input[pipeid - 1] = '\0';
    command_list = split_command_name_param(input, -1);
    if (command_list == NULL)
        return NULL;
    command_list->next = split_command_name_param(input + pipeid + 1, -1);
    return command_list;
}

static command_t *split_with_number_pipe(char *input, int numberpipe_id, int number)
{
    input[numberpipe_id] = '\0';
    return split_command_name_param(input, number);
}

#ifdef __DEBUG_MODE_ON
static void print_command(command_t *command)
{
    while (command)
    {
        printf("command name: %s\n", command->data.name);
        printf("in numberpipe: %d\n", command->input_numpipe_id);
        printf("out numberpipe: %d\n", command->output_numpipe_id);
        printf("numberpipe: %d\n", command->numberpipe);
        printf("bin_command: %d\n", command->bin_command);
        printf("param count: %d\n", command->data.param_count);
        printf("type: %d\n", command->type);
        printf("param:\n");
        for (int i = 0; i < command->data.param_count; i++)
            printf("  %s\n", command->data.parameter[i]);
        command = command->next;
    }
}
#endif // __DEBUG_MODE_ON

command_t *parse_input(char *input)
{
    command_t *command_list = NULL;
    int normal_pipe_exist = find_normal_pipe(input);
    int number_pipe_exist = find_number_pipe(input);              // ... | ...
    int number_pipe_number = atoi(input + number_pipe_exist + 1); // ... |2

    if (normal_pipe_exist != -1)
    {
        command_list = split_with_normal_pipe(input, normal_pipe_exist);
        command_list->type = COMMAND_TYPE_NORMAL_PIPE;
    }
    else if (number_pipe_exist != -1)
    {
        command_list = split_with_number_pipe(input, number_pipe_exist, number_pipe_number);
        command_list->type = COMMAND_TYPE_SINGLE;
        if (command_list->output_numpipe_id == -1)
        {
            printf("No more empty numpipe buffer\n");
            return NULL;
        }
    }
    else
    {
        command_list = split_command_name_param(input, -1);
        if (strncmp(command_list->data.name, "quit", strlen("quit")) == 0)
            command_list->type = COMMAND_TYPE_QUIT;
        else
            command_list->type = COMMAND_TYPE_SINGLE;
    }
    return command_list;
}

void free_command(command_t *cmd)
{
    command_t *next;
    while (cmd)
    {
        next = cmd->next;
        free(cmd);
        cmd = next;
    }
}