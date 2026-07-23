# include "codexion.h"


int check_values(char **av)
{
    if (valid_int(av[1]) == -1)
        return (printf("Error in number_of_coders: %s", av[1]), 0);
    if (valid_long(av[2]) == -1)
        return (printf("Error in Time of burnout: %s", av[2]), 0);
    if (valid_long(av[3]) == -1)
        return (printf("Error in Time to compile: %s", av[3]), 0);
    if (valid_long(av[4]) == -1)
        return (printf("Error in Time to debug: %s", av[4]), 0);
    if (valid_long(av[5]) == -1)
        return (printf("Error in Time to refactor: %s", av[5]), 0);
    if (valid_int(av[6]) == -1)
        return (printf("Error in number_of_compiles_required: %s", av[6]), 0);
    if (valid_long(av[7]) == -1)
        return (printf("Error in dongle_cooldown: %s", av[7]), 0);
    if (!check_scheduler(av[8]))
        return (printf("Error in scheduler type: %s", av[8]), 0);
    return 1;
}