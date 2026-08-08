#include "codexion.h"


struct timespec ms_to_timespec(long ms)
{
    struct timespec ts;

    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000L;
    return ts;
}

t_waiter *create_waiter()
{
    t_waiter *waiter;

    waiter = (t_waiter *) malloc (sizeof(t_waiter));
    if (!waiter)
        return NULL;
    waiter->coder = NULL;
    waiter->sss = 0;
    return waiter;
}