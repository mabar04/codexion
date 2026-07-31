#include "codexion.h"


struct timespec ms_to_timespec(long ms)
{
    struct timespec ts;

    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000L;
    return ts;
}