# include "codexion.h"

void free_sim(t_sim *sim)
{
    size_t i;

    i = 0;
    while (i < (*sim).number_of_dongles)
    {
        pthread_mutex_destroy(&((*sim).dongles[i].mutex));
        pthread_cond_destroy(&((*sim).dongles[i].cond));
        i++;
    }
    free((sim->coders));
    free((sim->dongles));
    pthread_mutex_destroy(&(*sim).print_mutex);
    pthread_mutex_destroy(&(*sim).stop_mutex);
    free(sim);
    
}

void print_sim(t_sim *sim)
{
    if (!sim)
        return;

    printf("========== Simulation ==========\n");

    printf("Time to compile : %ld ms\n", sim->time_to_compile);
    printf("Time to debug   : %ld ms\n", sim->time_to_debug);
    printf("Time to refactor: %ld ms\n", sim->time_to_refactor);
    printf("Time to burnout : %ld ms\n", sim->time_to_burnout);
    printf("Required compiles: %d\n", sim->required_compiles);
    printf("Time to cooldown: %ld ms\n", sim->time_to_cooldown);
    printf("Start time      : %ld\n", sim->start_time);

    printf("Number of coders : %zu\n", sim->number_of_coders);
    printf("Number of dongles: %zu\n", sim->number_of_dongles);

    printf("Stop            : %d\n", sim->stop);

    printf("Coders pointer  : %p\n", (void *)sim->coders);
    printf("Dongles pointer : %p\n", (void *)sim->dongles);

    printf("================================\n");
}

void free_coder_mutex(t_coder *coders, size_t k)
{
    size_t i;

    i = 0;
    while(i < k)
    {
        pthread_mutex_destroy(&(coders[i].last_compile_mutex));
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