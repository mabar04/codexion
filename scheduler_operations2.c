#include "codexion.h"

t_coder *heap_top(t_heap *heap)
{
    return heap->queqe[0];
}

int heap_empty(t_heap *heap)
{
    if (heap->filled == 0)
        return 1;
    return 0;
}

size_t heap_size(t_heap *heap)
{
    return heap->filled;
} 

void heap_destroy(t_heap *heap)
{
    free(heap);
}

// t_waiter *create_waiter()
// {
//     t_waiter *waiter;
//     waiter = (t_waiter *)malloc (sizeof(t_waiter));
//     if (!waiter)
//         return (NULL);
//     waiter->coder = NULL;
//     waiter->sss = 0;
//     return waiter;
// }