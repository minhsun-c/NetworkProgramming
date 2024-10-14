#include "includes.h"

#define NUMPIPE_BUFFER_COUNT 10
#define EMPTY_BUFFER_INFO -1
#define NUMPIPE_NOW 1

int *numpipe_buffer;

static int create_tmp_file()
{
    FILE *file;
    char filename[32];
    for (int i = 0; i < NUMPIPE_BUFFER_COUNT; i++)
    {
        sprintf(filename, "tmp/buf%d.txt", i);
        file = fopen(filename, "w");
        fclose(file);
    }
    return 1;
}

static int find_empty_buffer()
{
    for (int i = 0; i < NUMPIPE_BUFFER_COUNT; i++)
    {
        if (numpipe_buffer[i] == EMPTY_BUFFER_INFO)
            return i;
    }
    return -1;
}

int init_numpipe()
{
    numpipe_buffer = (int *)malloc(sizeof(int) * NUMPIPE_BUFFER_COUNT);
    if (numpipe_buffer == NULL)
    {
        ERR_MSG_MEM();
        return -1;
    }
    memset(numpipe_buffer, -1, sizeof(int) * NUMPIPE_BUFFER_COUNT);
    create_tmp_file();
    return 1;
}

int insert_numpipe_info(int cycle_after)
{
    if (cycle_after <= 0)
    {
        return -1;
    }
    int next_empty = find_empty_buffer();
    if (next_empty == -1)
        return -1;
    numpipe_buffer[next_empty] = cycle_after;
    return next_empty;
}

#ifdef __DEBUG_MODE_ON
static void print_buffer()
{
    printf("NUMPIPE: \n");
    for (int i = 0; i < NUMPIPE_BUFFER_COUNT; i++)
    {
        printf("%d ", numpipe_buffer[i]);
    }
    printf("\n");
}
#endif // __DEBUG_MODE_ON

int do_numpipe()
{
    int ret = -1;
    for (int i = 0; i < NUMPIPE_BUFFER_COUNT; i++)
    {
        if (ret == -1 && numpipe_buffer[i] == NUMPIPE_NOW)
        {
            ret = i;
        }
        else if (numpipe_buffer[i] >= 0)
            numpipe_buffer[i]--;
    }
    return ret;
}

int clear_numpipe(int id)
{
    numpipe_buffer[id] = EMPTY_BUFFER_INFO;
    return 1;
}

int close_numpipe_buffer()
{
    free(numpipe_buffer);
    numpipe_buffer = NULL;
    return 1;
}