#include "codexion.h"

void write_lastcompile(t_coder *coder)
{
    pthread_mutex_lock(&coder->state_mutex);
    coder->last_compile_start = get_time_ms();
    pthread_mutex_unlock(&coder->state_mutex);
}