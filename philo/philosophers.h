#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

# define FORK "🍴"
# define FORK_AND_PLATE "🍽 🍝"
# define SLEEP "🛌 💤"
# define THINK "🤔 💭"
# define CHECK "✅"
# define DEATH "💀💀💀"

typedef struct s_philosopher
{
	int					position;
	int					is_eating;
	int					is_thinking;
	int					left_fork;
	int					right_fork;
	int					how_many_times_eat;
	int					how_many_times_must_eat;
	unsigned long		last_eat;
	pthread_mutex_t		eating;
	pthread_mutex_t		mutex_philo;
	pthread_t			pthr;
	struct s_storage	*stor;
}	t_philosopher;

typedef struct s_input_argv
{
	int	number_of_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	number_of_times_each_philosopher_must_eat;
}	t_input_argv;

typedef struct s_storage
{
	t_philosopher	*philosopher;
	t_input_argv	*input;
	pthread_mutex_t	*arr_of_forks;
	pthread_mutex_t	print;
	pthread_mutex_t	someone_dead;
	pthread_mutex_t	someone_thinking;
	int				is_dead;
	int				total_number_of_meals;
	int				how_many_times_eat;
	unsigned long	time_start;
}	t_storage;

long			ft_atoi(const char *nptr);
int				ft_putstr_fd(char *s, int fd);
unsigned long	my_time(void);
int				ft_exit(t_storage *storage);
int				initialization(t_storage *storage, int argc, char **argv);
void			*check_is_dead(void *ptr);
int				launch_thread(t_storage *storage);

#endif
