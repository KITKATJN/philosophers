#include "philosophers.h"

static void ft_exit3(t_storage *storage, int i)
{
	if (storage->arr_of_forks != 0)
	{
		while (i < storage->input->number_of_philosophers)
			pthread_mutex_destroy(&storage->arr_of_forks[i++]);
		free(storage->arr_of_forks);
	}
}

static void ft_exit2(t_storage *storage, int i)
{
	if (storage->philosopher != 0)
	{
		while (i < storage->input->number_of_philosophers)
		{
			pthread_mutex_destroy(&storage->philosopher[i].eating);
			pthread_mutex_destroy(&storage->philosopher[i].mutex_philo);
			i++;
		}
		free(storage->philosopher);
	}
}

int ft_exit(t_storage *storage)
{
	ft_exit2(storage, 0);
	ft_exit3(storage, 0);
	pthread_mutex_destroy(&storage->someone_dead);
	pthread_mutex_destroy(&storage->print);
	return (1);
}
