/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_start.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabar <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:33:17 by mabar             #+#    #+#             */
/*   Updated: 2026/08/11 15:58:09 by mabar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_simulation_running(t_sim *sim)
{
	int	i;

	pthread_mutex_lock(&sim->stop_mutex);
	i = sim->stop;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (i);
}

static int	run_simualtion(t_coder *coder)
{
	sim_printf(coder, get_time_ms() - coder->sim->start_time,
		"has taken a dongle");
	sim_printf(coder, get_time_ms() - coder->sim->start_time,
		"has taken a dongle");
	heap_pop(coder->right_dongle->heap);
	heap_pop(coder->left_dongle->heap);
	compile(coder);
	increment_compiles(coder);
	release_dongle_cooldown(coder->right_dongle);
	release_dongle_cooldown(coder->left_dongle);
	if (read_numbercompiles(coder) == coder->sim->required_compiles)
	{
		modify_finished(coder, 1);
		return (1);
	}
	debug(coder);
	refactor(coder);
	return (0);
}

static int	wait_for_dongle(t_dongle *dongle, t_coder *coder)
{
	struct timespec	ts;

	pthread_mutex_lock(&dongle->mutex);
	while (dongle->is_used || get_time_ms() < dongle->available_at)
	{
		heap_push(coder, dongle->heap);
		if (check_simulation_running(dongle->sim))
		{
			pthread_mutex_unlock(&dongle->mutex);
			return (0);
		}
		if (dongle->is_used)
		{
			pthread_cond_wait(&dongle->cond, &dongle->mutex);
		}
		else
		{
			ts = ms_to_timespec(dongle->available_at);
			pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
		}
	}
	pthread_mutex_unlock(&dongle->mutex);
	return (1);
}

int	coder_simulation(t_coder *coder)
{
	int	left;
	int	right;

	while (!check_simulation_running(coder->sim))
	{
		left = try_acquire_dongle(coder->left_dongle, coder);
		if (!left)
		{
			if (!wait_for_dongle(coder->left_dongle, coder))
				return (0);
			continue ;
		}
		right = try_acquire_dongle(coder->right_dongle, coder);
		if (!right)
		{
			release_dongle(coder->left_dongle);
			if (!wait_for_dongle(coder->right_dongle, coder))
				return (0);
			continue ;
		}
		if (run_simualtion(coder) == 1)
			return (1);
	}
	return (0);
}
