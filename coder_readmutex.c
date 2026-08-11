/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_readmutex.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabar <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:33:16 by mabar             #+#    #+#             */
/*   Updated: 2026/08/11 15:58:08 by mabar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	read_finished(t_coder *coder)
{
	int	finished;

	pthread_mutex_lock(&coder->state_mutex);
	finished = coder->finished;
	pthread_mutex_unlock(&coder->state_mutex);
	return (finished);
}

void	modify_finished(t_coder *coder, int value)
{
	pthread_mutex_lock(&coder->state_mutex);
	coder->finished = value;
	pthread_mutex_unlock(&coder->state_mutex);
}

int	read_numbercompiles(t_coder *coder)
{
	int	compiles;

	pthread_mutex_lock(&coder->state_mutex);
	compiles = coder->number_compiles;
	pthread_mutex_unlock(&coder->state_mutex);
	return (compiles);
}

void	increment_compiles(t_coder *coder)
{
	pthread_mutex_lock(&coder->state_mutex);
	coder->number_compiles++;
	pthread_mutex_unlock(&coder->state_mutex);
}

long	read_lastcompile(t_coder *coder)
{
	long	a;

	pthread_mutex_lock(&coder->state_mutex);
	a = coder->last_compile_start;
	pthread_mutex_unlock(&coder->state_mutex);
	return (a);
}
