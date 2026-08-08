#include "codexion.h"

static t_coder *initialise_coders(t_sim *sim, size_t number_of_coders)
{
    t_coder *coders;
    size_t i; 

    i = 0;
    coders = (t_coder *) malloc(sizeof(t_coder) * number_of_coders);
    if (!coders)
        return NULL;
    while (i < number_of_coders)
    {
        coders[i].coder_id = i + 1;
        coders[i].sim = sim;
        coders[i].number_compiles = 0;
        coders[i].last_compile_start = sim->start_time;
        coders[i].finished = 0;
        if (pthread_mutex_init(&coders[i].state_mutex, NULL) != 0)
        {
            free_coder_mutex(coders, i);
            free(coders);
            return NULL;
        }
        i++;
    }
    return coders;
}

static t_dongle *coders_to_dongles(t_sim *sim)
{
    t_dongle *dongles;
    size_t i; 

    i = 0;
    dongles = (t_dongle *) malloc((sizeof(t_dongle) * (*sim).number_of_dongles));
    if (!dongles)
        return NULL;
    while ( i < (*sim).number_of_coders)
    {
        (*sim).coders[i].right_dongle = &dongles[(i + 1) % (*sim).number_of_coders];
        (*sim).coders[i].left_dongle = &dongles[i];
        i++;
    }
    return dongles;
}

int initialize_dongles(t_sim *sim)
{
    size_t i;

    i = 0; 
    while (i < (*sim).number_of_dongles)
    {
        (*sim).dongles[i].dongle_id = i + 1;
        (*sim).dongles[i].is_used = 0;
        (*sim).dongles[i].available_at = 0;
        (*sim).dongles[i].owner = NULL;
        sim->dongles[i].sim = sim;
        sim->dongles[i].heap = create_heap();
        if (sim->dongles[i].heap == NULL)
            return 0;
        sim->dongles[i].heap->type = sim->type;
        if(create_mutex_cond(&(sim->dongles[i])) == 0)
        {
            clear_mutexex(sim, i);
            free(sim->dongles[i].heap);
            return 0;
        }
        i++;
    }
    return 1;
}

int    init_sim(t_sim *sim, char **argv)
{
    sim->number_of_coders = valid_int(argv[0]);
    sim->number_of_dongles = valid_int(argv[0]);
    sim->time_to_burnout = valid_long(argv[1]);
    sim->time_to_compile = valid_long(argv[2]);
    sim->time_to_debug = valid_long(argv[3]);
    sim->time_to_refactor = valid_long(argv[4]);
    sim->required_compiles = valid_int(argv[5]);
    sim->time_to_cooldown = valid_long(argv[6]);
    sim->type = argv[7];
    sim->stop = 0;
    sim->start_time = get_time_ms();
    sim->coders = initialise_coders(sim, sim->number_of_coders);
    if (!sim->coders)
        return 0;
    sim->dongles = coders_to_dongles(sim);
    if (!sim->dongles)
        return (free(sim->coders), 0);
    if (initialize_dongles(sim) == 0)
    {
        free(sim->coders);
        free(sim->dongles);
        return 0;
    }
    if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
    {
        clear_mutexex(sim, sim->number_of_dongles);
        free_coder_mutex(sim->coders, sim->number_of_coders);
        free(sim->coders);
        free(sim->dongles);
        return 0;
    }
    if (pthread_mutex_init(&sim->stop_mutex, NULL) != 0)
    {
        pthread_mutex_destroy(&sim->print_mutex);
        clear_mutexex(sim, sim->number_of_dongles);
        free_coder_mutex(sim->coders, sim->number_of_coders);
        free(sim->coders);
        free(sim->dongles);
        return 0;
    }
    return 1;
}