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

#include "codexion.h"

void	print_sim_state(t_sim *sim)
{
	size_t	i;

	if (!sim)
		return ;

	printf("\n");
	printf("=========================================================\n");
	printf("                 CURRENT SIMULATION STATE\n");
	printf("=========================================================\n");

	printf("Simulation:\n");
	printf("  start_time          : %ld\n", sim->start_time);
	printf("  current_time        : %ld\n", get_time_ms());
	printf("  elapsed             : %ld\n", get_time_ms() - sim->start_time);
	printf("  stop                : %d\n", sim->stop);

	printf("\nConfiguration:\n");
	printf("  coders              : %zu\n", sim->number_of_coders);
	printf("  dongles             : %zu\n", sim->number_of_dongles);
	printf("  compile             : %ld\n", sim->time_to_compile);
	printf("  debug               : %ld\n", sim->time_to_debug);
	printf("  refactor            : %ld\n", sim->time_to_refactor);
	printf("  burnout             : %ld\n", sim->time_to_burnout);
	printf("  cooldown            : %ld\n", sim->time_to_cooldown);
	printf("  required_compiles   : %d\n", sim->required_compiles);

	printf("\n-------------------- CODERS -----------------------------\n");

	i = 0;
	while (i < sim->number_of_coders)
	{
		t_coder *c = &sim->coders[i];

		pthread_mutex_lock(&c->state_mutex);

		printf("Coder %zu\n", c->coder_id);
		printf("    address           : %p\n", (void *)c);
		printf("    thread            : %lu\n", (unsigned long)c->thread);
		printf("    compiles          : %d\n", c->number_compiles);
		printf("    finished          : %d\n", c->finished);
		printf("    last_compile      : %ld\n", c->last_compile_start);

		if (c->left_dongle)
			printf("    left dongle       : %zu\n",
				c->left_dongle->dongle_id);
		else
			printf("    left dongle       : NULL\n");

		if (c->right_dongle)
			printf("    right dongle      : %zu\n",
				c->right_dongle->dongle_id);
		else
			printf("    right dongle      : NULL\n");

		pthread_mutex_unlock(&c->state_mutex);

		printf("\n");
		i++;
	}

	printf("-------------------- DONGLES ----------------------------\n");

	i = 0;
	while (i < sim->number_of_dongles)
	{
		t_dongle *d = &sim->dongles[i];

		pthread_mutex_lock(&d->mutex);

		printf("Dongle %zu\n", d->dongle_id);
		printf("    address           : %p\n", (void *)d);
		printf("    used              : %d\n", d->is_used);
		printf("    cooldown          : %ld\n", d->sim->time_to_cooldown);
		printf("    available_at      : %ld\n", d->available_at);

		if (d->owner)
			printf("    owner             : %zu\n", d->owner->coder_id);
		else
			printf("    owner             : NULL\n");

		pthread_mutex_unlock(&d->mutex);

		printf("\n");
		i++;
	}

	printf("=========================================================\n\n");
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