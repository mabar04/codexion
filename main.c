#include "codexion.h"

int main(int ac, char **av)
{
    t_sim *sim;
    size_t i;

    i = 0;
    if (ac != 9)
    {
        printf("Missing arguments\n");
        return 2;
    }
    printf("Starting the program\n");
    if (check_values(av) == 0)
        return 2;
    sim = (t_sim *) malloc(sizeof(t_sim));
    if (!sim)
        return 3;
    if (init_sim(sim, av + 1) == 0)
        return (printf("Error in the inistialization\n"), 4);
    print_sim(sim);
    while (i < sim->number_of_coders)
    {
        printf("Coder %lu: Left-Dongle-> %lu | Right-dongle->%lu\n", (*sim).coders[i].coder_id,
        (*sim).coders[i].left_dongle->dongle_id,(*sim).coders[i].right_dongle->dongle_id);
        i++;
    }
    free_sim(sim);
    return 1;
}