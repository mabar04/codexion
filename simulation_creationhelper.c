/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_creationhelper.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabar <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 20:01:18 by mabar             #+#    #+#             */
/*   Updated: 2026/08/11 20:04:45 by mabar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	cleanup_dongles(t_sim *sim, size_t count)
{
	size_t	j;

	j = 0;
	while (j < count)
	{
		pthread_mutex_destroy(&sim->dongles[j].mutex);
		pthread_cond_destroy(&sim->dongles[j].cond);
		heap_destroy(sim->dongles[j].heap);
		j++;
	}
}

static int	init_dongle(t_sim *sim, size_t i)
{
	sim->dongles[i].dongle_id = i + 1;
	sim->dongles[i].is_used = 0;
	sim->dongles[i].available_at = 0;
	sim->dongles[i].owner = NULL;
	sim->dongles[i].sim = sim;
	sim->dongles[i].heap = create_heap();
	if (!sim->dongles[i].heap)
		return (0);
	sim->dongles[i].heap->type = sim->type;
	if (!create_mutex_cond(&sim->dongles[i]))
	{
		heap_destroy(sim->dongles[i].heap);
		sim->dongles[i].heap = NULL;
		return (0);
	}
	return (1);
}

int	initialize_dongles(t_sim *sim)
{
	size_t	i;

	i = 0;
	while (i < sim->number_of_dongles)
	{
		if (!init_dongle(sim, i))
		{
			cleanup_dongles(sim, i);
			return (0);
		}
		i++;
	}
	return (1);
}
