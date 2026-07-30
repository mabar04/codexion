#include "codexion.h" 

void awake_coders(t_sim *sim)
{
    size_t i; 
    
    i = 0; 
    while (i < sim -> number_of_dongles)
    {
        pthread_cond_broadcast(&sim->dongles[i].cond); 
        i++;
    }
} 

int check_all_finished(t_sim *sim)
{
    size_t i;
    int finished_number;

    finished_number = 0;
    i = 0;
    while (i < sim->number_of_coders)
    {
        if (sim->coders[i].finished == 1)
            finished_number++;
        i++;
    }
    if (finished_number == sim->number_of_coders)
        return 1;
    return 0;
}

void *check_burnout(void *a)
{
    t_sim * sim; 
    size_t i; 
    long last_time; 
    long current_time; 
    
    sim = (t_sim *)a; 
    while (!check_simulation_running(sim))
    {
        current_time = get_time_ms(); 
        i = 0; 
        while (i < sim->number_of_coders)
        {
            pthread_mutex_lock(&sim->coders[i].state_mutex); 
            last_time = current_time - sim->coders[i].last_compile_start; 
            pthread_mutex_unlock(&sim->coders[i].state_mutex); 
            if (last_time > sim->time_to_burnout || check_all_finished(sim))
            {
                pthread_mutex_lock(&sim->stop_mutex); 
                sim->stop = 1; 
                pthread_mutex_unlock(&sim->stop_mutex); 
                awake_coders(sim); 
                return NULL; 
            }
            i++; 
        }
        msleep(1); 
    } return NULL; 
} 

int monitor_thread(t_sim * sim)
{
    pthread_t monitor; 
    
    if (pthread_create(&monitor, NULL, &check_burnout, (void *)sim) != 0)
            return 0; 
        if (pthread_join(monitor, NULL) != 0) 
        return 0; 
}