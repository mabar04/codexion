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
    t_water *waiter;
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

t_coder *heap_pop(t_heap *heap)
{
   
}
