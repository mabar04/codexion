#include "codexion.h"

int acquire_dongle(t_dongle *dongle) 
{
    struct timespec ts;

    if(check_simulation_running(dongle->sim))
        return 0;
    pthread_mutex_lock(&dongle->mutex);
    if (dongle->owner != NULL)
        sim_printf(dongle->owner, dongle->available_at, "dongle is availaible at");
    while(dongle->is_used || get_time_ms() < dongle->available_at) 
    {
        if (dongle->is_used)
            pthread_cond_wait(&dongle->cond, &dongle->mutex);
        else if (get_time_ms() < dongle->available_at)
        {
            ts = ms_to_timespec(dongle->available_at);
            pthread_cond_timedwait(&dongle->cond, &dongle->mutex,&ts);
        }

        if(check_simulation_running(dongle->sim)) 
        {
            pthread_mutex_unlock(&dongle->mutex); 
            return 0;
        }
    }
    dongle->is_used = 1;
    
    pthread_mutex_unlock(&dongle->mutex);
    return 1; 
} 

void release_dongle(t_dongle *dongle) 
{ 
    pthread_mutex_lock(&dongle->mutex); 
    dongle->is_used = 0;
    pthread_cond_signal(&dongle->cond);
    pthread_mutex_unlock(&dongle->mutex); 
} 

int try_acquire_dongle(t_dongle *dongle) 
{
    int success; 
    
    if (check_simulation_running(dongle->sim))
        return 0;
    success = 0;
    pthread_mutex_lock(&dongle->mutex); 
    if (dongle->is_used == 0 && get_time_ms() >= dongle->available_at) 
    {
        dongle->is_used = 1;
        success = 1; 
    } 
    pthread_mutex_unlock(&dongle->mutex); 
    return success; 
}


void release_dongle_cooldown(t_dongle *dongle) 
{ 
    pthread_mutex_lock(&dongle->mutex); 
    dongle->is_used = 0;
    dongle->available_at = get_time_ms() + dongle->sim->time_to_cooldown;
    pthread_cond_signal(&dongle->cond);
    pthread_mutex_unlock(&dongle->mutex);
} 
