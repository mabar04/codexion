#include "codexion.h"


void compile(t_coder *coder)
{
    sim_printf(coder, get_time_ms()- coder->sim->start_time, "is compiling");
    coder->last_compile_start = coder->sim->start_time - get_time_ms();
    msleep(coder->sim->time_to_compile);
}

void refactor(t_coder *coder)
{
    sim_printf(coder, get_time_ms() - coder->sim->start_time, "is debugging");
    msleep(coder->sim->time_to_refactor);
}

void debug(t_coder *coder)
{
    sim_printf(coder, get_time_ms()- coder->sim->start_time, "is refactoring");
    msleep(coder->sim->time_to_debug);
}