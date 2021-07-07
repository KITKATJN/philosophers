#include "philosophers.h"

void	init_philosopher(t_storage *storage)
{
	int	i;

	i = 0;
	while (i < storage->input->number_of_philosophers)
	{
		storage->philosopher[i].stor = storage;
		storage->philosopher[i].is_eating = 0;
		storage->philosopher[i].position = i + 1;
		storage->philosopher[i].how_many_times_eat = 0;
		storage->philosopher[i].how_many_times_must_eat
			= storage->input->number_of_times_each_philosopher_must_eat;
		storage->philosopher[i].left_fork = i;
		storage->philosopher[i].right_fork = (i + 1)
			% storage->input->number_of_philosophers;
		pthread_mutex_init(&storage->philosopher[i].mutex_philo, 0);
		pthread_mutex_init(&storage->philosopher[i].eating, 0);
		pthread_mutex_init(&storage->arr_of_forks[i], 0);
		i++;
	}
}

int	check_error(t_storage *storage)
{
	storage->philosopher = 0;
	storage->arr_of_forks = 0;
	storage->is_dead = 0;
	if (storage->input->number_of_times_each_philosopher_must_eat != -1)
		storage->total_number_of_meals
			= storage->input->number_of_times_each_philosopher_must_eat
			* storage->input->number_of_philosophers;
	else
		storage->total_number_of_meals = -1;
	if (storage->input->time_to_die < 60 || storage->input->time_to_eat < 60
		|| storage->input->time_to_sleep < 60)
		return (ft_putstr_fd("So little time for eat or die or sleep\n", 1));
	if (storage->input->number_of_philosophers > 200)
		return (ft_putstr_fd("To many philosophers\n", 1));
	storage->philosopher = malloc(sizeof(*storage->philosopher)
			* storage->input->number_of_philosophers);
	if (storage->philosopher == 0)
		return (ft_putstr_fd("Error with malloc\n", 1));
	storage->arr_of_forks = malloc(sizeof(*storage->arr_of_forks)
			* storage->input->number_of_philosophers);
	if (storage->arr_of_forks == 0)
		return (ft_putstr_fd("Error with malloc\n", 1));
	return (0);
}

int	initialization(t_storage *storage, int argc, char **argv)
{
	if (ft_atoi(argv[1]) < 0 || ft_atoi(argv[2]) < 0
		|| ft_atoi(argv[3]) < 0 || ft_atoi(argv[4]) < 0)
		ft_putstr_fd("One of arguments is negative\n", 1);
	storage->input->number_of_philosophers = ft_atoi(argv[1]);
	storage->input->time_to_die = ft_atoi(argv[2]);
	storage->input->time_to_eat = ft_atoi(argv[3]);
	storage->input->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
	{
		if (ft_atoi(argv[5]) <= 0)
			return (ft_putstr_fd
				("Number_of_times_each_philosopher_must_eat must be positive\n",
					1));
		storage->input->number_of_times_each_philosopher_must_eat
			= ft_atoi(argv[5]);
	}
	else
		storage->input->number_of_times_each_philosopher_must_eat = -1;
	if (check_error(storage) == 1)
		return (1);
	pthread_mutex_init(&storage->someone_dead, 0);
	pthread_mutex_init(&storage->print, 0);
	storage->time_start = my_time();
	init_philosopher(storage);
	return (launch_thread(storage));
}
