#include "codexion.h"

void *coder_routine(void *a) 
{ 
    t_coder *coder;
   
    while(pthread_mutex_lock(&coder->sim->stop_mutex), !coder->sim->stop)
    {

    }
    return NULL;
}


static void initialise_threads(t_sim *sim)
{
    size_t i;
    size_t j;

    i = 0;
    while (i < sim->number_of_coders)
    {
        if (pthread_create(&sim->coders[i].thread, NULL, coder_routine, (void *)&sim->coders[i]) != 0)
            break;
        i++;
    }
    j = 0;
    while( j < i)
    {
        pthread_join(sim->coders[j].thread, NULL);
        j++;
    }
}

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
        return (printf("Error in the inistialization\n"), free(sim), 4);
    printf("Simulation Starts\n");
    initialise_threads(sim);
    free_sim(sim);
    return 1;
}