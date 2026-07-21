#include "codexion.h"

static t_coder *initialise_coders(t_sim *sim, int ac)
{
    t_coder coders[ac];

    for (size_t i = 0; i < ac; i++)
    {
        coders[i].coder_id = i + 1;
        coders[i].sim = sim;
        coders[i].number_compiles = 0;
    }
}

void *init_sim(t_sim *sim,int ac, char **argv)
{
    (*sim).number_of_coders = valid_int(argv[0]);
    (*sim).number_of_dongles = valid_int(argv[0]);
    (*sim).time_to_burnout = valid_long(argv[1]);
    (*sim).time_to_compile = valid_long(argv[2]);
    (*sim).time_to_debug = valid_long(argv[3]);
    (*sim).time_to_refactor = valid_long(argv[4]);
    (*sim).required_compiles = valid_int(argv[5]);
    (*sim).time_to_cooldown = valid_long(argv[6]);
    (*sim).coders = (t_coder *)malloc(sizeof(t_coder) * valid_int(argv[0]));
    if ((*sim).coders == NULL)
        return NULL;
    (*sim).dongles = (t_dongle *)malloc(sizeof(t_dongle) * valid_int(argv[0]));
    if ((*sim).dongles == NULL)
        return NULL;
    (*sim).coders = initialise_coders(sim, ac);
}