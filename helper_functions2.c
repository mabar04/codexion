/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabar <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:33:16 by mabar             #+#    #+#             */
/*   Updated: 2026/08/11 20:04:45 by mabar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	create_mutex_cond(t_dongle *dongle)
{
	if (pthread_mutex_init(&dongle->mutex, NULL) != 0)
		return (0);
	if (pthread_cond_init(&dongle->cond, NULL) != 0)
	{
		pthread_mutex_destroy(&dongle->mutex);
		return (0);
	}
	return (1);
}

void	clear_mutexex(t_sim *sim, size_t k)
{
	size_t	i;

	i = 0;
	while (i < k)
	{
		pthread_mutex_destroy(&(sim->dongles[i].mutex));
		pthread_cond_destroy(&(sim->dongles[i].cond));
		heap_destroy(sim->dongles[i].heap);
		i++;
	}
}

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000L));
}

void	sim_printf(t_coder *coder, long time, char *task)
{
	pthread_mutex_lock(&coder->sim->print_mutex);
	if (check_simulation_running(coder->sim))
	{
		if (strcmp(task, "burned out") == 0)
			printf("%ld %zu %s\n", time, coder->coder_id, task);
		pthread_mutex_unlock(&coder->sim->print_mutex);
		return ;
	}
	printf("%ld %zu %s\n", time, coder->coder_id, task);
	pthread_mutex_unlock(&coder->sim->print_mutex);
}

void	msleep(t_sim *sim, long ms)
{
	long	start;

	start = get_time_ms();
	while (!check_simulation_running(sim))
	{
		if (get_time_ms() - start >= ms)
			break ;
		usleep(20);
	}
}
