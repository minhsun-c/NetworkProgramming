#include "includes.h"

extern builtin_command_t *builtin;

static void print_builtin_command()
{
    builtin_command_t *ptr = builtin;
    while (ptr)
    {
        printf("    %s\n", ptr->name);
        ptr = ptr->next;
    }
}

void myhelp(void *param)
{
    (void)param;
    printf("  Commands List:\n");
    print_builtin_command();
    printf("    command in bin\n");
    printf("    normal pipe\n");
    printf("    number pipe\n");
}

void myquit(void *param)
{
    (void)param;
    printf("  Close Shell\n");
    exit(EXIT_SUCCESS);
}

static builtin_command_t *
construct_builtin_command_node(char *name, void (*fptr)(void *))
{
    builtin_command_t *cmd =
        (builtin_command_t *)malloc(sizeof(builtin_command_t));
    if (cmd == NULL)
    {
        ERR_MSG_MEM();
        return NULL;
    }
    cmd->fptr = fptr;
    cmd->next = NULL;
    strncpy(cmd->name, name, strlen(name));
    return cmd;
}

static builtin_command_t *
insert_builtin_command(char *name, void (*fptr)(void *))
{
    builtin_command_t *cmd = construct_builtin_command_node(name, fptr);
    if (cmd == NULL)
        return NULL;
    if (builtin == NULL)
        return cmd;
    builtin_command_t *ptr = builtin;
    while (ptr && ptr->next)
        ptr = ptr->next;
    ptr->next = cmd;
    return builtin;
}

builtin_command_t *find_builtin_command(char *name)
{
    builtin_command_t *ptr = builtin;
    while (ptr)
    {
        if (strncmp(name, ptr->name, strlen(ptr->name)) == 0)
            return ptr;
        ptr = ptr->next;
    }
    return NULL;
}

int init_builtin_command()
{
    builtin = insert_builtin_command("help", myhelp);
    builtin = insert_builtin_command("setenv", mysetenv);
    builtin = insert_builtin_command("printenv", myprintenv);
    builtin = insert_builtin_command("quit", myquit);
    return 1;
}