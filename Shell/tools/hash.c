#include <stdio.h>

unsigned long hash(const char *str)
{
    unsigned long counter = 5381;
    int c;

    while ((c = *str++))
        counter = ((counter << 5) + counter) + c; // counter * 33 + c

    return counter;
}

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        int hashno = hash(argv[1]);
        printf("Hash Number: %d\n", hashno);
    }
    else
    {
        perror("Error!\n");
    }
}