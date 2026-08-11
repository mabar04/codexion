/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabar <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:33:16 by mabar             #+#    #+#             */
/*   Updated: 2026/08/11 15:58:08 by mabar            ###   ########.fr       */
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

static void	initialise_threads(t_sim *sim)
{
	size_t	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL, coder_routine,
				(void *)&sim->coders[i]) != 0)
			break ;
		i++;
	}
}

void	join_threads(t_sim *sim)
{
	size_t	j;

	j = 0;
	while (j < sim->number_of_coders)
	{
		pthread_join(sim->coders[j].thread, NULL);
		j++;
	}
}

int	main(int ac, char **av)
{
	t_sim		*sim;
	pthread_t	monitor;

	if (ac != 9)
	{
		printf("Missing arguments\n");
		return (2);
	}
	if (check_values(av) == 0)
		return (2);
	sim = (t_sim *)malloc(sizeof(t_sim));
	if (!sim)
		return (3);
	if (init_sim(sim, av + 1) == 0)
		return (printf("Error in the inistialization\n"), free(sim), 4);
	if (monitor_thread(sim, &monitor) == -1)
		return (free_sim(sim), 20);
	initialise_threads(sim);
	if (pthread_join(monitor, NULL) != 0)
	{
		printf("[MAIN] ERROR: pthread_join failed.\n");
		return (free_sim(sim), 80);
	}
	join_threads(sim);
	return (free_sim(sim), 1);
}
