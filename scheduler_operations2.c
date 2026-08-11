/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_operations2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabar <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:33:16 by mabar             #+#    #+#             */
/*   Updated: 2026/08/11 20:04:45 by mabar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_waiter	*heap_top(t_heap *heap)
{
	return (heap->queqe[0]);
}

int	heap_empty(t_heap *heap)
{
	if (heap->filled == 0)
		return (1);
	return (0);
}

size_t	heap_size(t_heap *heap)
{
	return (heap->filled);
}

void	heap_destroy(t_heap *heap)
{
	size_t	i;

	if (!heap)
		return ;
	i = 0;
	while (i < heap->filled)
		free(heap->queqe[i++]);
	free(heap);
}
