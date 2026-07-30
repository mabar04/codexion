#include "codexion.h" 

int check_simulation_running(t_sim *sim) 
{ 
    int i; 
    
    pthread_mutex_lock(&sim->stop_mutex); 
    i = sim->stop; 
    pthread_mutex_unlock(&sim->stop_mutex); 
    return i;
} 

void coder_simulation(t_coder *coder) 
{
    while(!check_simulation_running(coder->sim)) 
    { 
        if (!acquire_dongle(coder->left_dongle))
            return;
        if (coder->number_compiles == coder->sim->required_compiles)
        {
            coder->finished = 1;
            return;
        }
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
            msleep(500);
        }
    }   
}