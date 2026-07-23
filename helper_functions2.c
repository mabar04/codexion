#include "codexion.h"


int create_mutex_cond(t_dongle *dongle)
{
    if(pthread_mutex_init(&dongle->mutex, NULL) != 0)
        return 0;
    if (pthread_cond_init(&dongle->cond, NULL) != 0)
    {
        pthread_mutex_destroy(&dongle->mutex);
        return 0;
    }
    return 1;
}

void clear_mutexex(t_sim *sim, size_t k)
{
    size_t i;

    i = 0;
    while(i < k)
    {
        pthread_mutex_destroy(&(sim->dongles[i].mutex));
        pthread_cond_destroy(&(sim->dongles[i].cond));
    }
}