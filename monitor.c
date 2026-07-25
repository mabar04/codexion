#include "codexion.h"

void *check_burnout(void *)
{
    return NULL;
}

pthread_t *monitor_thread()
{
    pthread_t monitor;
    if (pthread_create(&monitor, NULL, &check_burnout, NULL) != 0)
        return NULL;
    

}