#include "codexion.h"

static t_coder *initialise_coders(t_sim *sim, int number_of_coders)
{
    t_coder *coders;

    coders = (t_coder *) malloc(sizeof(t_coder) * number_of_coders);
    if (!coders)
        return NULL;
    for (size_t i = 0; i < number_of_coders; i++)
    {
        coders[i].coder_id = i + 1;
        coders[i].sim = sim;
        coders[i].number_compiles = 0;
    }
    return coders;
}

static t_dongle *coders_to_dongles(t_sim *sim)
{
    t_dongle *dongles;

    dongles = (t_dongle *) malloc((sizeof(t_dongle) * (*sim).number_of_dongles));
    if (!dongles)
        return NULL;
    for (size_t i = 0; i < (*sim).number_of_coders; i++)
    {
        (*sim).coders[i].right_dongle = &dongles[(i + 1) % (*sim).number_of_coders];
        (*sim).coders[i].left_dongle = &dongles[i];
    }
    return dongles;
}

void initialize_dongles(t_sim *sim)
{
    for (size_t i = 0; i < (*sim).number_of_dongles; i++)
    {
        (*sim).dongles[i].dongle_id = i + 1;
    }
    
}

int    init_sim(t_sim *sim,int ac, char **argv)
{
    (*sim).number_of_coders = valid_int(argv[0]);
    (*sim).number_of_dongles = valid_int(argv[0]);
    (*sim).time_to_burnout = valid_long(argv[1]);
    (*sim).time_to_compile = valid_long(argv[2]);
    (*sim).time_to_debug = valid_long(argv[3]);
    (*sim).time_to_refactor = valid_long(argv[4]);
    (*sim).required_compiles = valid_int(argv[5]);
    (*sim).time_to_cooldown = valid_long(argv[6]);
    (*sim).coders = initialise_coders(sim, (*sim).number_of_coders);
    if (!(*sim).coders)
        return 0;
    (*sim).dongles = coders_to_dongles(sim);
    if (!(*sim).dongles)
        return (free_coders((*sim).coders, (*sim).number_of_coders), 0);
    initialize_dongles(sim);
    return 1;
}