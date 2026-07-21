#include "codexion.h"

int main(int ac, char **av)
{
    t_sim sim;
    for (int  i = 0; i < ac; i++)
    {
        printf("%s\n", av[i]);
    }
    init_sim(&sim,ac,av);
    
}