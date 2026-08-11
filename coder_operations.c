/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabar <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:33:16 by mabar             #+#    #+#             */
/*   Updated: 2026/08/11 15:58:08 by mabar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->is_used = 0;
	pthread_cond_signal(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}

int	try_acquire_dongle(t_dongle *dongle, t_coder *coder)
{
	int	success;

	if (check_simulation_running(dongle->sim))
		return (0);
	success = 0;
	pthread_mutex_lock(&dongle->mutex);
	if (dongle->is_used == 0 && get_time_ms() >= dongle->available_at
		&& (heap_empty(dongle->heap) || coder == heap_top(dongle->heap)->coder))
	{
		dongle->is_used = 1;
		success = 1;
	}
	pthread_mutex_unlock(&dongle->mutex);
	return (success);
}

void	release_dongle_cooldown(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->is_used = 0;
	dongle->available_at = get_time_ms() + dongle->sim->time_to_cooldown;
	pthread_cond_signal(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}
