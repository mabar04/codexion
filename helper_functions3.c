#include "codexion.h"


struct timespec ms_to_timespec(long ms)
{
    struct timespec ts;

    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000L;
    return ts;
}


void sim_completed(t_sim *sim)
{
    pthread_mutex_lock(&sim->print_mutex);
    printf("Simulation completed");
    pthread_mutex_unlock(&sim->print_mutex);
}
