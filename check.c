# include "codexion.h"

int ft_putnbr (char s)
{
    int a;
    a = s - '0';
    return a;

}

int check_valid(char *s)
{
    if(*s == '\0' || !s)
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
    while (s[i] >= 9 && s[i] <= 13)
        i++;
    while (s[i])
    {
        
        if (*s == '+')
            s++;
        if (result * 10 + ft_putnbr(s[i]) > LONG_MAX)
            return -1;
        result = result * 10 + ft_putnbr(s[i]);
        i++;
    }
    return (long)result;
}

int valid_int(char *s)
{
    long a;
    a = valid_long(s);
    if (a == -1 || a > INT_MAX)
        return -1;
    return (int)a;
}

int check_scheduler(char *s)
{
    if (!strcmp(s, "fifo") || !strcmp(s, "edf"))
        return 1;
    return 0;
}