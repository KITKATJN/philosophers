#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>

typedef struct s_philosopher
{
	int					position;
	int					is_eating;
	int					is_thinking;
	int					left_fork;
	int					right_fork;
	int					how_many_times_eat;
	unsigned long		last_eat;
	pthread_mutex_t		eating;
	pthread_mutex_t		mutex_philo;
	pthread_t			pthr;
	struct s_storage	*stor;
}	t_philosopher;

typedef struct s_input_argv
{
	size_t	number_of_philosophers;
	size_t	time_to_die;
	size_t	time_to_eat;
	size_t	time_to_sleep;
	size_t	number_of_times_each_philosopher_must_eat;
}	t_input_argv;

typedef struct s_storage
{
	t_philosopher	*philosopher;
	t_input_argv	*input;
	pthread_mutex_t	*arr_of_forks;
	pthread_mutex_t	someone_write;
	pthread_mutex_t	someone_dead;
	int				is_dead;
	size_t			time_start;
}	t_storage;

long			ft_atoi(const char *nptr);
int				ft_putstr_fd(char *s, int fd);
unsigned long	my_time(void);

#endif
