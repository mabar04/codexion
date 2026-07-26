#include "codexion.h"

void *check_burnout(void *a)
{
    t_sim *sim;
    size_t i;
    long last_time;
    long current_time;

    sim = (t_sim *)a;
    
    while (!sim->stop)
    {
        pthread_mutex_lock(&sim->stop_mutex);
        if (sim->stop)
        {
            pthread_mutex_unlock(&sim->stop_mutex);
            exit;
        }
        pthread_mutex_unlock(&sim->stop_mutex);
        current_time = get_time_ms();
        i = 0;
        while (i < sim->number_of_coders)
        {
            pthread_mutex_lock(&sim->coders[i].last_compile_mutex);
            last_time = current_time - sim->coders[i].last_compile_start;
            pthread_mutex_unlock(&sim->coders[i].last_compile_mutex);
            if (last_time > sim->time_to_burnout)
            {
                pthread_mutex_lock(&sim->stop_mutex);
                sim->stop = 1;
                pthread_mutex_unlock(&sim->stop_mutex);
                exit;
            }
            i++;
        }
        msleep(1);
    }
    return NULL;
}

int monitor_thread(t_sim *sim)
{
    pthread_t monitor;
    if (pthread_create(&monitor, NULL, &check_burnout, (void*)sim) != 0)
        return 0;
    
    if (pthread_join(monitor,NULL) != 0)
        return 0;

}