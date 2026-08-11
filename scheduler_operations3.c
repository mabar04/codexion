/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_operations3.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabar <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:33:16 by mabar             #+#    #+#             */
/*   Updated: 2026/08/11 15:58:09 by mabar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_heap	*create_heap(void)
{
	t_heap	*heap;
	size_t	i;

	i = 0;
	heap = (t_heap *)malloc(sizeof(t_heap));
	if (!heap)
		return (NULL);
	while (i < MAX_CODERS)
	{
		heap->queqe[i] = NULL;
		i++;
	}
	heap->filled = 0;
	return (heap);
}

int	node_exists(t_heap *heap, size_t index)
{
	if (index < heap->filled && heap->queqe[index] != NULL)
		return (1);
	return (0);
}

void	swap_coders(t_heap *heap, int index1, int index2)
{
	t_waiter	*tmp;

	tmp = heap->queqe[index1];
	heap->queqe[index1] = heap->queqe[index2];
	heap->queqe[index2] = tmp;
}
