#include "codexion.h"

t_heap *create_heap()
{
    t_heap *heap;
    size_t i;

    i = 0;
    heap = (t_heap *)malloc(sizeof(t_heap));
    if (!heap)
        return NULL;
    while (i < MAX_CODERS)
    {
        heap->queqe[i] = NULL;
        i++;
    }
    heap->filled = 0;
    return heap;
}

int compare_coders(t_heap *heap, int parent, int child)
{
   
    if (heap->queqe[parent]->sss > heap->queqe[child]->sss)
        return 1;
    return 0;
}

int heap_push(t_coder *coder, t_heap *heap)
{
    t_waiter *waiter;
    size_t i;
    int child;
    int parent_index;

    i = heap->filled;
    if (heap->filled >= MAX_CODERS)
        return 0;
    heap->queqe[i] = coder;
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
            break;
    }
    heap->filled++; 
    return 1;
}

int node_exists(t_heap *heap, int index)
{
    if (index < heap->filled && heap->queqe[index] != NULL)
        return 1;
    return 0;
}

void swap_coders(t_heap *heap, int index1, int index2)
{
    t_waiter *tmp;
    tmp = heap->queqe[index1];
    heap->queqe[index1] = heap->queqe[index2];
    heap->queqe[index2] = tmp;
}

t_waiter *heap_pop(t_heap *heap)
{
    t_waiter *top;
    t_waiter *smallest;
    size_t i;
    int smallest_index;

    if (heap->filled == 0)
        return NULL;
    top = heap->queqe[0];
    heap->queqe[0] = heap->queqe[heap->filled - 1];
    i = 0;
    heap->filled--;
    while (1)
    {
        smallest_index = 0;
        smallest = heap->queqe[i];
        if (node_exists(heap, i*2 + 1) && heap->queqe[i*2 + 1]->sss < smallest->sss)
        {
            smallest = heap->queqe[i*2 + 1];
            smallest_index = i*2 + 1;
        }
        if (node_exists(heap, i*2 + 2) && heap->queqe[i*2 + 2]->sss < smallest->sss)
        {
            smallest = heap->queqe[i*2 + 2];
            smallest_index = i*2 + 2;
        }    
        if (smallest == heap->queqe[i])
            break;
        swap_coders(heap, i , smallest_index);
        i = smallest_index;        
    }
    heap->queqe[heap->filled] = NULL;
    return top;
}
