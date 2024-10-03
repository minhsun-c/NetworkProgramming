#include "includes.h"
#define COMPLEX_SETENV
#define MAX_ENV_COUNT 64
#define MAX_LINE_COUNT 256

extern char **myenviron;
char **next_empty;
bool written = true;

char **find_name(char *name)
{
    size_t name_len = strlen(name);
    for (char **env = myenviron; *env; ++env)
    {
        if (strncmp(*env, name, name_len) == 0 && (*env)[name_len] == '=')
            return env;
    }
    return NULL;
}

static bool split_param(char *param, char *name, char *value)
{
    int idx_name = 0, idx_value = 0;
    bool flag;
    for (flag = true; *param && *param != ' '; ++param)
    {
        if (*param == '=')
        {
            flag = false;
            continue;
        }
        if (flag)
            name[idx_name++] = *param;
        else
            value[idx_value++] = *param;
    }
    if (flag)
        return false;
    name[idx_name] = 0;
    value[idx_value] = 0;
    return true;
}

void free_env()
{
    if (myenviron == NULL)
        return;
    for (char **env = myenviron; env; ++env)
    {
        if (*env)
            free(*env);
    }
    free(myenviron);
    myenviron = NULL;
    next_empty = NULL;
    return;
}

bool read_env()
{
    free_env();
    FILE *file;
    if ((file = fopen("env.txt", "r")) == NULL)
    {
        file = fopen("env.txt", "w");
        fprintf(file, "PATH=./bin\n");
        fclose(file);
        file = fopen("env.txt", "r");
    }
    if ((myenviron = (char **)malloc(sizeof(char *) * MAX_ENV_COUNT)) == NULL)
    {
        perror("Memory allocated failed");
        return false;
    }
    memset(myenviron, 0, sizeof(char *) * MAX_ENV_COUNT);

    char line[MAX_LINE_COUNT];
    char **env = myenviron;
    while (fgets(line, MAX_LINE_COUNT, file) != NULL)
    {
        if ((*env = (char *)malloc(MAX_LINE_COUNT)) == NULL)
        {
            perror("Memory allocated failed");
            return false;
        }
        strncpy(*env, line, MAX_LINE_COUNT);
        (*env)[strlen(*env) - 1] = 0;
        ++env;
    }
    next_empty = env;
    written = false;
    fclose(file);
    return true;
}

static void write_env(char *name, char *value)
{
    if ((*next_empty = (char *)malloc(MAX_LINE_COUNT)) == NULL)
    {
        perror("Memory allocated failed");
        return;
    }
    strncpy(*next_empty, name, strlen(name));
    (*next_empty)[strlen(name)] = '=';
    strncpy((*next_empty) + strlen(name) + 1, value, strlen(value));
    (*next_empty)[strlen(*next_empty)] = 0;
    ++next_empty;
}

static bool update_env(char **target, char *value)
{
    int targetlen = strlen(*target);
    if (sizeof(*target) < strlen(value) + targetlen)
    {
        if (realloc(*target, sizeof(*target) * 2) == NULL)
        {
            perror("Memory allocated failed");
            return false;
        }
    }
    (*target)[targetlen++] = ':';
    strncpy((*target) + targetlen, value, strlen(value));
    (*target)[strlen(*target)] = 0;
    return true;
}

static void write_back_env()
{
    FILE *file;
    if ((file = fopen("env.txt", "w")) == NULL)
    {
        perror("Environment variable not exist");
        return;
    }
    for (char **env = myenviron; *env; ++env)
    {
        fprintf(file, "%s\n", *env);
    }
    written = false;
    fclose(file);
}

void mysetenv(char *param)
{
    if (param[0] == 0)
    {
        perror("Parameters not set");
        return;
    }
    if (myenviron == NULL)
    {
        read_env();
    }
    char name[32], value[256];
    char **target;
    if (split_param(param, name, value) == false)
    {
        perror("Parameter not included =");
        return;
    }
    if ((target = find_name(name)) == NULL)
        write_env(name, value);
    else
        update_env(target, value);
    written = true;
    write_back_env();
}

void myprintenv()
{
    if (myenviron == NULL)
        read_env();
    for (char **env = myenviron; *env; ++env)
    {
        printf("%s\n", *env);
    }
}