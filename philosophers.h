#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

#include <unistd.h>
#include <pthread.h>

typedef struct s_philosopher
{
	int	position;
	int	is_eating;
	int	is_thinking;
	int	is_eating;
	int	left_fork;
	int	right_fork;
	int	how_many_times_eat;
	pthread_mutex_t	eating;
	pthread_mutex_t	mutex_philo;
}	t_philosopher;

typedef struct	s_input_argv
{
	size_t	number_of_philosophers;
	size_t	time_to_die;
	size_t	time_to_eat;
	size_t	time_to_sleep;
	size_t	number_of_times_each_philosopher_must_eat;
}	t_input_argv;

typedef struct	s_storage
{
	t_philosopher	*philosopher;
	t_input_argv	*input;
	pthread_mutex_t	*arr_of_forks;
	pthread_mutex_t	someone_write;
	pthread_mutex_t	someone_dead;
}	t_storage;

long	ft_atoi(const char *nptr);
int		main(int argc, char ** argv);
int		ft_putstr_fd(char *s, int fd);

#endif
