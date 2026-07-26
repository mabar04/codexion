#include "codexion.h"

void acquire_dongle(t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->mutex);
    while(dongle->is_used)
        pthread_cond_wait(&dongle->cond, &dongle->mutex);
    dongle->is_used = 1;
    pthread_mutex_unlock(&dongle->mutex);
}

void release_dongle(t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->mutex);
    dongle->is_used = 0;
    pthread_cond_broadcast(&dongle->cond);
    pthread_mutex_unlock(&dongle->mutex);
}

int try_acquire_dongle(t_dongle *dongle)
{
    int success;

    success = 0;
    pthread_mutex_lock(&dongle->mutex);
    if (dongle->is_used == 0)
    {
            dongle->is_used = 1;
            success = 1;
    }
    pthread_mutex_unlock(&dongle->mutex);
    return success;
}