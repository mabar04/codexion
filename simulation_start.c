#include "codexion.h"


void coder_simulation(t_coder *coder)
{
    while(!coder->sim->stop)
    {
        acquire_dongle(coder->left_dongle);
        if (try_acquire_dongle(coder->right_dongle))
        {
            compile(coder);
            coder->number_compiles++;
            release_dongle(coder->right_dongle);
            release_dongle(coder->left_dongle);
            debug(coder);
            refactor(coder);

        }
        else
        {
            release_dongle(coder->left_dongle);
            usleep(500);
        }
    }
}