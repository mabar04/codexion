#include "codexion.h"

int main(int ac, char **av)
{
    t_sim sim;
    if (ac != 9)
    {
        printf("Missing arguments\n");
        return 2;
    }
    printf("Starting the program\n");
    check_values(av);
    init_sim(&sim, ac, av);
    return 1;
}