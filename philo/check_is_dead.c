#include "philosophers.h"

static void	*check_is_dead2(t_philosopher *philo)
{
	philo->stor->is_dead = 1;
	pthread_mutex_unlock(&philo->eating);
	pthread_mutex_lock(&philo->stor->someone_dead);
	printf("%lu ms %d died %s\n", (my_time() - philo->stor->time_start),
		philo->position, DEATH);
	return (0);
}

void	*check_is_dead(void *ptr)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)ptr;
	while (philo->stor->is_dead == 0)
	{
		usleep(100);
		pthread_mutex_lock(&philo->eating);
		if (my_time() - philo->last_eat
			> (unsigned long)philo->stor->input->time_to_die
			&& philo->stor->is_dead == 0)
			return (check_is_dead2(philo));
		if (philo->how_many_times_eat >= philo->how_many_times_must_eat
			&& philo->how_many_times_must_eat != -1
			&& philo->stor->is_dead == 0)
		{
			printf("%lu ms %d eat the right number of times %s\n",
				(my_time() - philo->stor->time_start),
				philo->position, CHECK);
			return (0);
		}
		pthread_mutex_unlock(&philo->eating);
	}
	return (0);
}
