# include "codexion.h"

void free_sim(t_sim *sim)
{
    size_t i;

    i = 0;
    while (i < (*sim).number_of_dongles)
    {
        pthread_mutex_destroy(&((*sim).dongles[i].mutex));
        pthread_cond_destroy(&((*sim).dongles[i].cond));
		free(sim->dongles[i].heap);
        i++;
    }
    free((sim->coders));
    free((sim->dongles));
    pthread_mutex_destroy(&(*sim).print_mutex);
    pthread_mutex_destroy(&(*sim).stop_mutex);
    free(sim);
    
}

void free_coder_mutex(t_coder *coders, size_t k)
{
    size_t i;

    i = 0;
    while(i < k)
    {
        pthread_mutex_destroy(&(coders[i].state_mutex));
        i++;
    }
}

void wake_up_coders(t_sim *sim)
{
    size_t i;

    i = 0;
    while (i < sim->number_of_dongles)
    {
        pthread_cond_broadcast(&sim->dongles[i].cond);
        i++;
    }
}