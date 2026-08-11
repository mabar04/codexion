/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabar <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:33:16 by mabar             #+#    #+#             */
/*   Updated: 2026/08/11 20:04:45 by mabar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_all_finished(t_sim *sim)
{
	size_t	i;
	int		finished_number;

	finished_number = 0;
	i = 0;
	while (i < sim->number_of_coders)
	{
		if (read_finished(&sim->coders[i]))
			finished_number++;
		i++;
	}
	return (finished_number == (int)sim->number_of_coders);
}

static void	help_stop(t_sim *sim, long current_time, size_t i)
{
	pthread_mutex_lock(&sim->stop_mutex);
	if (sim->stop)
	{
		pthread_mutex_unlock(&sim->stop_mutex);
		return ;
	}
	sim->stop = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
	awake_coders(sim);
	sim_printf(&sim->coders[i], current_time - sim->start_time, "burned out");
}

static void	*all_finished(t_sim *sim)
{
	pthread_mutex_lock(&sim->stop_mutex);
	if (sim->stop)
	{
		pthread_mutex_unlock(&sim->stop_mutex);
		return (NULL);
	}
	sim->stop = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
	awake_coders(sim);
	return (NULL);
}

void	*check_burnout(void *a)
{
	t_sim	*sim;
	size_t	i;
	long	last_time;
	long	current_time;

	sim = (t_sim *)a;
	while (!check_simulation_running(sim))
	{
		current_time = get_time_ms();
		i = 0;
		while (i < sim->number_of_coders)
		{
			last_time = current_time - read_lastcompile(&sim->coders[i]);
			if (last_time > sim->time_to_burnout)
				return (help_stop(sim, current_time, i), NULL);
			i++;
		}
		if (check_all_finished(sim))
		{
			if (all_finished(sim) == NULL)
				return (NULL);
		}
		usleep(1);
	}
	return (NULL);
}

int	monitor_thread(t_sim *sim, pthread_t *monitor)
{
	if (pthread_create(monitor, NULL, check_burnout, sim) != 0)
	{
		printf("[MAIN] ERROR: pthread_create failed.\n");
		return (-1);
	}
	return (1);
}
