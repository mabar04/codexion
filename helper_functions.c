# include "codexion.h"

void free_coders(t_coder *coders, int number_coders)
{
    size_t i;

    for (i = 0; i < number_coders; i++)
        free(&coders[i]);
    free(coders);
}