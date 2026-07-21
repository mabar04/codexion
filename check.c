# include "codexion.h"


int check_valid(char *s)
{
    if(*s == '\0')
        return 0;

    while (*s >= 9 && *s <= 13 )
        s++;

    if(*s == '+')
        s++;

    while (*s >= '0' && *s <= '9' )
        s++;

    if(*s == '\0')
        return 1;

    return 0;
}

long valid_long(char *s)
{
    unsigned long result;
    size_t i = 0;
    result = 0; 
    while (s[i])
    {
        while (*s >= 9 && *s <= 13 )
            s++;
        if (*s == '+')
            s++;
        if (result * 10 + atoi(s[i]) > LONG_MAX)
            return -1;
        result = result * 10 + atoi(s[i]);
        i++;
    }
    return (long)result;
}

int check_time_values(char **av, int ac)
{
    for (int i = 0; i < ac; i++)
    {

    }
}