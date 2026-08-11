/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabar <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:33:16 by mabar             #+#    #+#             */
/*   Updated: 2026/08/11 20:04:45 by mabar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coder_routine(void *a)
{
	t_coder	*coder;

	coder = (t_coder *)a;
	while (!check_simulation_running(coder->sim))
	{
		if (coder_simulation(coder) == 1)
			break ;
	}
	return (NULL);
}

static size_t	initialise_threads(t_sim *sim)
{
	size_t	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL, coder_routine,
				(void *)&sim->coders[i]) != 0)
			return (i);
		i++;
	}
	return (i);
}

void	join_threads(t_sim *sim, size_t count)
{
	size_t	j;

	j = 0;
	while (j < count)
	{
		pthread_join(sim->coders[j].thread, NULL);
		j++;
	}
}

static int	run_simulation(t_sim *sim)
{
	pthread_t	monitor;
	size_t		created;

	if (monitor_thread(sim, &monitor) == -1)
		return (20);
	created = initialise_threads(sim);
	if (created != sim->number_of_coders)
	{
		printf("[MAIN] ERROR: pthread_create failed.\n");
		pthread_mutex_lock(&sim->stop_mutex);
		sim->stop = 1;
		pthread_mutex_unlock(&sim->stop_mutex);
		awake_coders(sim);
	}
	if (pthread_join(monitor, NULL) != 0)
	{
		printf("[MAIN] ERROR: pthread_join failed.\n");
		join_threads(sim, created);
		return (80);
	}
	join_threads(sim, created);
	return (0);
}

int	main(int ac, char **av)
{
	t_sim	*sim;
	int		status;

	if (ac != 9)
		return (printf("Missing arguments\n"), 2);
	if (!check_values(av))
		return (2);
	sim = malloc(sizeof(t_sim));
	if (!sim)
		return (3);
	if (!init_sim(sim, av + 1))
		return (printf("Error in the initialization\n"), free(sim), 4);
	status = run_simulation(sim);
	free_sim(sim);
	return (status);
}
