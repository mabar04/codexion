#ifndef CODEXION_H
#define CODEXION_H


#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include <pthread.h>
#include <limits.h>
#include <string.h>

typedef struct s_sim        t_sim;
typedef struct s_dongle     t_dongle;
typedef struct s_coder      t_coder;
// typedef enum s_scheduler    t_scheduler;

typedef struct s_coder
{
    int coder_id;
    int number_compiles;
    long last_compile_start;

    pthread_t thread;

    t_dongle   *left_dongle;
    t_dongle   *right_dongle;
    t_sim *sim;

}   t_coder;


typedef struct s_dongle
{
    int dongle_id;
    long cooldown_time;
    long                available_at;

    pthread_mutex_t     mutex;
    pthread_cond_t      cond;
} t_dongle;

typedef struct s_sim
{
    long time_to_compile;
    long time_to_debug;
    long time_to_refactor;
    long time_to_burnout;
    int required_compiles;
    long time_to_cooldown;

    int number_of_coders;
    int number_of_dongles;

    int stop;

    t_dongle *dongles;
    t_coder *coders;
    pthread_mutex_t print_mutex;
    pthread_mutex_t stop_mutex;
    // t_scheduler scheduler;

} t_sim;

// typedef enum s_scheduler
// {
//     FIFO,
//     EDF
// }   t_scheduler;

void init_sim(t_sim *sim,int ac, char **args);
int check_valid(char *s);
long valid_long(char *s);
int valid_int(char *s);
int check_scheduler(char *s);
void check_values(char **av);
void free_coders(t_coder *coders, int number_coders);
#endif