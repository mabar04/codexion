#include "codexion.h"


void compile(t_coder *coder)
{
    write_lastcompile(coder);
    sim_printf(coder, get_time_ms()- coder->sim->start_time, "is compiling");
    print_sim_state(coder->sim);
    msleep(coder->sim, coder->sim->time_to_compile);
}

void refactor(t_coder *coder)
{
    sim_printf(coder, get_time_ms() - coder->sim->start_time, "is refactoring");
    print_sim_state(coder->sim);
    msleep(coder->sim, coder->sim->time_to_refactor);
}

void debug(t_coder *coder)
{
    sim_printf(coder, get_time_ms() - coder->sim->start_time, "is debugging");
    print_sim_state(coder->sim);
    msleep(coder->sim, coder->sim->time_to_debug);
}