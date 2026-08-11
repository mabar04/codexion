/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_operations.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabar <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:33:16 by mabar             #+#    #+#             */
/*   Updated: 2026/08/11 15:58:09 by mabar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	compare_coders(t_heap *heap, int parent, int child)
{
	if (heap->queqe[parent]->sss > heap->queqe[child]->sss)
		return (1);
	return (0);
}

static int	help_initialise(t_coder *coder, t_heap *heap, size_t i)
{
	if (heap->filled >= MAX_CODERS)
		return (0);
	heap->queqe[i] = create_waiter();
	if (!heap->queqe[i])
		return (0);
	heap->queqe[i]->coder = coder;
	if (!strcmp(heap->type, "edf"))
		heap->queqe[i]->sss = coder->last_compile_start
			+ coder->sim->time_to_burnout;
	else
		heap->queqe[i]->sss = heap->filled;
	return (1);
}

int	heap_push(t_coder *coder, t_heap *heap)
{
	t_waiter	*waiter;
	size_t		i;
	int			child;
	int			parent_index;

	if (coder_in_heap(coder, heap))
		return (0);
	i = heap->filled;
	if (help_initialise(coder, heap, i) == 0)
		return (0);
	while (i > 0)
	{
		child = i;
		parent_index = (child - 1) / 2;
		if (compare_coders(heap, parent_index, child))
		{
			waiter = heap->queqe[parent_index];
			heap->queqe[parent_index] = heap->queqe[child];
			heap->queqe[child] = waiter;
			i = parent_index;
		}
		else
			break ;
	}
	return (heap->filled++, 1);
}

static void	helper_pop(int *smallest_index, t_waiter **smallest, t_heap *heap,
		size_t *i)
{
	while (1)
	{
		*smallest_index = 0;
		*smallest = heap->queqe[*i];
		if (node_exists(heap, (*i) * 2 + 1) && heap->queqe[(*i) * 2
				+ 1]->sss < (*smallest)->sss)
		{
			(*smallest) = heap->queqe[(*i) * 2 + 1];
			(*smallest_index) = (*i) * 2 + 1;
		}
		if (node_exists(heap, (*i) * 2 + 2) && heap->queqe[(*i) * 2
				+ 2]->sss < (*smallest)->sss)
		{
			(*smallest) = heap->queqe[(*i) * 2 + 2];
			(*smallest_index) = (*i) * 2 + 2;
		}
		if ((*smallest) == heap->queqe[(*i)])
			break ;
		swap_coders(heap, (*i), (*smallest_index));
		(*i) = (*smallest_index);
	}
}

t_waiter	*heap_pop(t_heap *heap)
{
	t_waiter	*top;
	t_waiter	*smallest;
	size_t		i;
	int			smallest_index;

	if (heap->filled == 0)
		return (NULL);
	top = heap->queqe[0];
	heap->queqe[0] = heap->queqe[heap->filled - 1];
	i = 0;
	heap->filled--;
	heap->queqe[heap->filled] = NULL;
	helper_pop(&smallest_index, &smallest, heap, &i);
	free(top);
	top = NULL;
	return (top);
}
