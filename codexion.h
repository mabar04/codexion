/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabar <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:33:42 by mabar             #+#    #+#             */
/*   Updated: 2026/08/09 14:33:42 by mabar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define MAX_CODERS 2

typedef struct s_sim	t_sim;
typedef struct s_dongle	t_dongle;
typedef struct s_coder	t_coder;
typedef struct s_heap	t_heap;
typedef struct s_waiter	t_waiter;

typedef struct s_coder
{
	size_t				coder_id;
	int					number_compiles;
	long				last_compile_start;
	int					finished;

	pthread_t			thread;

	t_dongle			*left_dongle;
	t_dongle			*right_dongle;
	t_sim				*sim;

	pthread_mutex_t		state_mutex;

}						t_coder;

typedef struct s_dongle
{
	size_t				dongle_id;
	long				available_at;
	int					is_used;

	pthread_mutex_t		mutex;
	pthread_cond_t		cond;
	t_coder				*owner;
	t_sim				*sim;
	t_heap				*heap;
}						t_dongle;

typedef struct s_sim
{
	long				time_to_compile;
	long				time_to_debug;
	long				time_to_refactor;
	long				time_to_burnout;
	int					required_compiles;
	long				time_to_cooldown;
	long				start_time;
	char				*type;
	size_t				number_of_coders;
	size_t				number_of_dongles;

	int					stop;

	t_dongle			*dongles;
	t_coder				*coders;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		stop_mutex;

}						t_sim;

typedef struct s_heap
{
	t_waiter			*queqe[2];
	char				*type;
	size_t				filled;
}						t_heap;

typedef struct s_waiter
{
	t_coder				*coder;
	long				sss;
}						t_waiter;

int						init_sim(t_sim *sim, char **args);
int						check_values(char **av);
int						check_valid(char *s);
long					valid_long(char *s);
int						valid_int(char *s);
int						check_scheduler(char *s);
void					free_sim(t_sim *sim);
void					print_sim_state(t_sim *sim);
int						create_mutex_cond(t_dongle *dongle);
void					clear_mutexex(t_sim *sim, size_t k);
void					release_dongle(t_dongle *dongle);
void					compile(t_coder *coder);
void					refactor(t_coder *coder);
void					debug(t_coder *coder);
long					get_time_ms(void);
void					sim_printf(t_coder *coder, long time, char *task);
void					msleep(t_sim *sim, long ms);
void					free_coder_mutex(t_coder *coders, size_t k);
int						check_simulation_running(t_sim *sim);
int						read_finished(t_coder *coder);
int						read_numbercompiles(t_coder *coder);
void					increment_compiles(t_coder *coder);
long					read_lastcompile(t_coder *coder);
void					modify_finished(t_coder *coder, int value);
void					write_lastcompile(t_coder *coder);
void					release_dongle_cooldown(t_dongle *dongle);
struct timespec			ms_to_timespec(long ms);
void					sim_completed(t_sim *sim);
void					*check_burnout(void *a);
int						monitor_thread(t_sim *sim, pthread_t *monitor);
int						check_all_finished(t_sim *sim);
t_heap					*create_heap(void);
int						heap_push(t_coder *coder, t_heap *heap);
t_waiter				*heap_pop(t_heap *heap);
int						acquire_dongle(t_dongle *dongle, t_coder *coder);
t_waiter				*heap_top(t_heap *heap);
int						heap_empty(t_heap *heap);
size_t					heap_size(t_heap *heap);
void					heap_destroy(t_heap *heap);
void					heap_info(t_heap *heap);
t_waiter				*create_waiter(void);
int						coder_simulation(t_coder *coder);
void					awake_coders(t_sim *sim);
int						node_exists(t_heap *heap, size_t index);
void					swap_coders(t_heap *heap, int index1, int index2);
int						try_acquire_dongle(t_dongle *dongle, t_coder *coder);
int						coder_in_heap(t_coder *coder, t_heap *heap);
#endif
