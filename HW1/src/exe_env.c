#include "includes.h"

#define ENV_NAME_NOT_FOUND -1
#define ENV_REDEFINE -2

extern env_t *env;

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

static int find_env_name(char *env_name)
{
    int target;
    int name_len = strlen(env_name);
    for (target = 0; target < MAX_ENV_SIZE; target++)
    {
        if (strncmp(env_name, env->name[target], name_len) == 0)
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

static int insert_env(char *env_name, char *env_data)
{
    int env_id = find_env_name(env_name);
    if (env_id == ENV_NAME_NOT_FOUND)
    {
        strncpy(env->name[env->next_empty], env_name, strlen(env_name));
        env_id = env->next_empty;
        env->next_empty++;
    }
    if (insert_env_node(env->list[env_id], env_data))
        return 1;
    else
    {
        printf("Insert Env-Variable Failed\n");
        return 0;
    }
}

static void print_env()
{
    for (int i = 0; i < env->next_empty; i++)
    {
        printf("[%s] = \n", env->name[i]);
        env_node_t *ptr = env->list[i]->next;
        while (ptr)
        {
            printf("\t%s\n", ptr->data);
            ptr = ptr->next;
        }
    }
}

static void print_env_with_name(char *env_name)
{
    int name_id = find_env_name(env_name);
    if (name_id == ENV_NAME_NOT_FOUND)
    {
        printf("Invalid Env. Name\n");
        return;
    }
    env_node_t *ptr = env->list[name_id]->next;
    printf("[%s] = \n", env->name[name_id]);
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
    insert_env_node(environ->list[0], "/bin");
    insert_env_node(environ->list[0], "/usr/bin");
    insert_env_node(environ->list[0], "./bin");
    environ->next_empty = 1;
    return environ;
}

void myprintenv(void *name)
{
    if (env == NULL)
    {
        printf("Env Variable Not Declared\n");
        return;
    }
    if (name == NULL)
        print_env();
    else
        print_env_with_name((char *)name);
}

void mysetenv(void *param)
{
    if (env == NULL)
    {
        return;
    }
    char *name;
    char *data;
    if (split_env_name_data((char *)param, &name, &data) == 0)
    {
        printf("Equal Sign not Included in setenv param\n");
        return;
    }
    insert_env(name, data);
    return;
}

int check_command(char *name, char *path)
{
    int path_id = find_env_name("PATH");
    char filename[1024];

    env_node_t *option = env->list[path_id]->next;
    while (option)
    {
        sprintf(filename, "%s/%s", strip_space(option->data), name);
        if (access(filename, X_OK) == 0)
        {
            strncpy(path, filename, strlen(filename));
            return 1;
        }
        option = option->next;
    }

    if (access(name, X_OK) == 0)
    {
        strncpy(path, filename, strlen(filename));
        return 1;
    }
    else
    {
        path[0] = '\0';
        return 0;
    }
}