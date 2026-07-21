# include "codexion.h"


void check_values(char **av)
{
    if (valid_int(av[1]) == -1)
        printf("Error in number_of_coders: %s", av[1]);
    if (valid_long(av[2]) == -1)
        printf("Error in Time of burnout: %s", av[2]);
    if (valid_long(av[3]) == -1)
        printf("Error in Time to compile: %s", av[3]);
    if (valid_long(av[4]) == -1)
        printf("Error in Time to debug: %s", av[4]);
    if (valid_long(av[5]) == -1)
        printf("Error in Time to refactor: %s", av[5]);
    if (valid_int(av[6]) == -1)
        printf("Error in number_of_compiles_required: %s", av[6]);
    if (valid_long(av[7]) == -1)
        printf("Error in dongle_cooldown: %s", av[7]);
    if (!check_scheduler(av[8]))
        printf("Error in scheduler type: %s", av[8]);
}