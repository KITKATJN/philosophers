#include "philosophers.h"

static void	output(t_philosopher *p, char *action, char *smile)
{
	if (p->stor->is_dead == 0)
	{
		pthread_mutex_lock(&p->stor->print);
		if (p->stor->is_dead == 0)
			printf("%lu ms %d %s %s\n", (my_time() - p->stor->time_start),
				p->position, action, smile);
		pthread_mutex_unlock(&p->stor->print);
	}
}

static void	going_sleep(int time_to_sleep, t_storage *storage)
{
	unsigned long	end;

	end = my_time() + time_to_sleep;
	while (storage->is_dead == 0 && my_time() < end)
		usleep(time_to_sleep);
}

static void	eating_spaghetti(t_philosopher *p)
{
	pthread_mutex_lock(&p->stor->arr_of_forks[p->right_fork]);
	output(p, "has taken a right fork", FORK);
	if (p->stor->input->number_of_philosophers == 1)
	{
		going_sleep(p->stor->input->time_to_die + 20, p->stor);
		pthread_mutex_unlock(&p->stor->arr_of_forks[p->right_fork]);
		return ;
	}
	pthread_mutex_lock(&p->stor->arr_of_forks[p->left_fork]);
	output(p, "has taken a left fork", FORK);
	pthread_mutex_lock(&p->eating);
	p->last_eat = my_time();
	output(p, "is eating", FORK_AND_PLATE);
	p->how_many_times_eat++;
	going_sleep(p->stor->input->time_to_eat, p->stor);
	pthread_mutex_unlock(&p->eating);
	pthread_mutex_unlock(&p->stor->arr_of_forks[p->right_fork]);
	pthread_mutex_unlock(&p->stor->arr_of_forks[p->left_fork]);
	output(p, "is sleeping", SLEEP);
	going_sleep(p->stor->input->time_to_sleep, p->stor);
	output(p, "is thinking", THINK);
}

static void	*life_cicl(void *ptr)
{
	t_philosopher	*philo;
	pthread_t		pid;

	philo = (t_philosopher *)ptr;
	philo->last_eat = my_time();
	if (pthread_create(&pid, 0, check_is_dead, philo) != 0)
		return (0);
	pthread_detach(pid);
	if (philo->position % 2 == 0 && philo->stor->input->time_to_eat > 1)
		going_sleep(philo->stor->input->time_to_eat * 0.9, philo->stor);
	while (philo->stor->is_dead == 0 && (philo->how_many_times_must_eat == -1
			|| philo->how_many_times_must_eat > philo->how_many_times_eat))
		eating_spaghetti(philo);
	return (0);
}

int	launch_thread(t_storage *storage)
{
	int		j;

	j = 0;
	while (j < storage->input->number_of_philosophers)
	{
		if (pthread_create(&storage->philosopher[j].pthr,
				0, life_cicl, (void*)&storage->philosopher[j]) != 0)
			return (1 + ft_putstr_fd("Can't create thread\n", 1));
		j++;
	}
	j = 0;
	while (j < storage->input->number_of_philosophers)
	{
		pthread_join(storage->philosopher[j].pthr, 0);
		j++;
	}
}
