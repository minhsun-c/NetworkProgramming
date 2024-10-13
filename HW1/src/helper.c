#include "includes.h"
#define ISSPACE(s) (s == ' ' || s == '\n' || s == '\t')

char *strip_space(char *str)
{
    int slen = strlen(str);
    char *ret = str;
    if (slen == 0)
        return NULL;
    int start, end;
    for (start = 0; start < slen; start++)
    {
        if (ISSPACE(str[start]))
            ret++;
        else
            break;
    }
    for (end = slen - 1; end >= start; end--)
    {
        if (ISSPACE(str[end]))
            str[end] = '\0';
        else
            break;
    }
    return ret;
}