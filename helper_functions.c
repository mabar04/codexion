/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabar <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:33:16 by mabar             #+#    #+#             */
/*   Updated: 2026/08/11 20:04:45 by mabar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_sim(t_sim *sim)
{
	size_t	i;

	i = 0;
	while (i < (*sim).number_of_dongles)
	{
		pthread_mutex_destroy(&((*sim).dongles[i].mutex));
		pthread_cond_destroy(&((*sim).dongles[i].cond));
		heap_destroy(sim->dongles[i].heap);
		i++;
	}
	free_coder_mutex(sim->coders, sim->number_of_coders);
	free((sim->coders));
	free((sim->dongles));
	pthread_mutex_destroy(&(*sim).print_mutex);
	pthread_mutex_destroy(&(*sim).stop_mutex);
	free(sim);
}

void	free_coder_mutex(t_coder *coders, size_t k)
{
	size_t	i;

	i = 0;
	while (i < k)
	{
		pthread_mutex_destroy(&(coders[i].state_mutex));
		i++;
	}
}

void	wake_up_coders(t_sim *sim)
{
	size_t	i;

	i = 0;
	while (i < sim->number_of_dongles)
	{
		pthread_cond_broadcast(&sim->dongles[i].cond);
		i++;
	}
}
