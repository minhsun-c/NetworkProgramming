unsigned int hash(const char *str)
{
    unsigned int counter = 5381;
    int c;

    while ((c = *str++))
        counter = ((counter << 5) + counter) + c; // counter * 33 + c

    return counter;
}