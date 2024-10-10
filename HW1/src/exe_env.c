#include "includes.h"

#define ENV_NAME_NOT_FOUND -1
#define ENV_REDEFINE -2

static int env_malloc_failed(env_t *environ, int end)
{
    for (int i = 0; i < end; i++)
        free(environ->list[i]);
    free(environ);
    return 1;
}

static env_node_t *construct_env_node(char *env_data)
{
    env_node_t *node;
    if ((node = (env_node_t *)malloc(sizeof(env_node_t))) == NULL)
    {
        ERR_MSG_MEM();
        return NULL;
    }
    strncpy(node->data, env_data, strlen(env_data));
    node->next = NULL;
    return node;
}

static int find_env_name(env_t *environ, char *env_name)
{
    int target;
    int name_len = strlen(env_name);
    for (target = 0; target < MAX_ENV_SIZE; target++)
    {
        if (strncmp(env_name, environ->name[target], name_len) == 0)
            break;
    }
    return (target == MAX_ENV_SIZE) ? ENV_NAME_NOT_FOUND : target;
}

static int insert_env_node(env_node_t *list, char *env_data)
{
    env_node_t *ptr = list;
    while (ptr && ptr->next)
    {
        if (ptr != list && strncmp(ptr->data, env_data, strlen(env_data)) == 0)
            return ENV_REDEFINE;
        ptr = ptr->next;
    }
    if (strncmp(ptr->data, env_data, strlen(env_data)) == 0)
        return ENV_REDEFINE;
    ptr->next = construct_env_node(env_data);
    return (ptr->next != NULL);
}

static int insert_env(env_t *environ, char *env_name, char *env_data)
{
    int env_id = find_env_name(environ, env_name);
    if (env_id == ENV_NAME_NOT_FOUND)
    {
        strncpy(environ->name[environ->next_empty], env_name, strlen(env_name));
        env_id = environ->next_empty;
        environ->next_empty++;
    }
    if (insert_env_node(environ->list[env_id], env_data))
        return 1;
    else
    {
        printf("Insert Env-Variable Failed\n");
        return 0;
    }
}

static void print_env(env_t *environ)
{
    for (int i = 0; i < environ->next_empty; i++)
    {
        printf("[%s] = \n", environ->name[i]);
        env_node_t *ptr = environ->list[i]->next;
        while (ptr)
        {
            printf("\t%s\n", ptr->data);
            ptr = ptr->next;
        }
    }
}

static void print_env_with_name(env_t *environ, char *env_name)
{
    int name_id = find_env_name(environ, env_name);
    if (name_id == ENV_NAME_NOT_FOUND)
    {
        printf("Invalid Env. Name\n");
        return;
    }
    env_node_t *ptr = environ->list[name_id]->next;
    printf("[%s] = \n", environ->name[name_id]);
    while (ptr)
    {
        printf("\t%s\n", ptr->data);
        ptr = ptr->next;
    }
}

static int split_env_name_data(char *input, char **name, char **data)
{
    char *eqsign = strchr(input, '=');
    if (eqsign != NULL)
    {
        *eqsign = '\0'; // Only modify if eqsign is not NULL
    }
    *name = input;
    *data = eqsign + 1;
    return 1;
}

env_t *construct_env()
{
    env_t *environ;
    if ((environ = (env_t *)malloc(sizeof(env_t))) == NULL)
    {
        ERR_MSG_MEM();
        return NULL;
    }
    for (int i = 0; i < MAX_ENV_SIZE; i++)
    {
        environ->list[i] = (env_node_t *)malloc(sizeof(env_node_t));
        if (environ->list[i] == NULL)
        {
            env_malloc_failed(environ, i);
            ERR_MSG_MEM();
            return NULL;
        }
        environ->list[i]->next = NULL;
    }
    strcpy(environ->name[0], "PATH");
    insert_env_node(environ->list[0], "./bin");
    environ->next_empty = 1;
    return environ;
}

void myprintenv(env_t *environ, char *name)
{
    if (environ == NULL)
    {
        printf("Env Variable Not Declared\n");
        return;
    }
    if (name == NULL)
        print_env(environ);
    else
        print_env_with_name(environ, name);
}

env_t *mysetenv(env_t *environ, char *param)
{
    if (environ == NULL)
    {
        environ = construct_env();
        if (environ == NULL)
        {
            ERR_MSG_MEM();
            return NULL;
        }
    }
    char *name;
    char *data;
    if (split_env_name_data(param, &name, &data) == 0)
    {
        printf("Equal Sign not Included in setenv param\n");
        return NULL;
    }
    insert_env(environ, name, data);
    return environ;
}