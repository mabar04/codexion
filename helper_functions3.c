/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabar <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:33:16 by mabar             #+#    #+#             */
/*   Updated: 2026/08/09 18:15:38 by mabar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

struct timespec	ms_to_timespec(long ms)
{
	struct timespec	ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	return (ts);
}

t_waiter	*create_waiter(void)
{
	t_waiter	*waiter;

	waiter = (t_waiter *)malloc(sizeof(t_waiter));
	if (!waiter)
		return (NULL);
	waiter->coder = NULL;
	waiter->sss = 0;
	return (waiter);
}

void	awake_coders(t_sim *sim)
{
	size_t	i;

	i = 0;
	while (i < sim->number_of_dongles)
	{
		pthread_cond_broadcast(&sim->dongles[i].cond);
		i++;
	}
}
