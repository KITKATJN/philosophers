#include "philosophers.h"

void *check_is_dead(void *ptr)
{
	t_philosopher *philo;

	philo = (t_philosopher*)ptr;
	while (philo->stor->is_dead == 0)
	{
		usleep(1000);
		pthread_mutex_lock(&philo->eating);
		if (my_time() - philo->last_eat > (unsigned long)philo->stor->input->time_to_die)
		{
			philo->stor->is_dead = 1;
			pthread_mutex_unlock(&philo->eating);
			pthread_mutex_lock(&philo->stor->someone_dead);
			printf("%lu ms %d died\n", (my_time() - philo->stor->time_start), philo->position);
			return (0);
		}
		if (philo->stor->how_many_times_eat >= philo->stor->input->number_of_times_each_philosopher_must_eat
			&& philo->stor->input->number_of_times_each_philosopher_must_eat != -1)
			return (0);
		pthread_mutex_unlock(&philo->eating);
	}
	return (0);
}

void eating_spaghetti(t_philosopher *philo)
{

}

void *life_cicl(void *ptr)
{
	t_philosopher	*philo;
	pthread_t		pid;

	philo = (t_philosopher*)ptr;
	philo->last_eat = my_time();
	if (pthread_create(&pid, 0, check_is_dead, philo) != 0)
		return (0);
	pthread_detach(pid);
	if (philo->position % 2 == 0 && philo->stor->input->number_of_times_each_philosopher_must_eat > 1 && philo->stor->is_dead == 0)
		usleep(philo->stor->input->time_to_eat * 0.88);
	while (philo->stor->is_dead == 0 && (philo->stor->input->number_of_times_each_philosopher_must_eat == -1 ||
		philo->stor->input->number_of_times_each_philosopher_must_eat > philo->how_many_times_eat))
		eating_spaghetti(philo);
	return (0);
}

int initialization(t_storage *storage, int argc, char **argv)
{
	if (ft_atoi(argv[1]) < 0 || ft_atoi(argv[2]) < 0 || ft_atoi(argv[3]) < 0 || ft_atoi(argv[4]) < 0)
		ft_putstr_fd("One of arguments is negative\n", 1);
	storage->input->number_of_philosophers = ft_atoi(argv[1]);
	storage->input->time_to_die = ft_atoi(argv[2]);
	storage->input->time_to_eat = ft_atoi(argv[3]);
	storage->input->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
	{
		if (ft_atoi(argv[5]) < 0)
			return(ft_putstr_fd("Number_of_times_each_philosopher_must_eat is negative\n", 1));
		storage->input->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	}
	else
		storage->input->number_of_times_each_philosopher_must_eat = -1;
	storage->philosopher = 0;
	storage->arr_of_forks = 0;
	storage->is_dead = 0;
	if (storage->input->number_of_philosophers < 2)
		return (ft_putstr_fd("Not enough philosopfhers\n", 1));
	if (storage->input->time_to_die < 60 || storage->input->time_to_eat < 60 || storage->input->time_to_sleep < 60)
		return(ft_putstr_fd("So little time for eat or die or sleep\n", 1));
	if (storage->input->number_of_philosophers > 200)
		return(ft_putstr_fd("To many philosophers\n", 1));
	storage->philosopher = malloc(sizeof(*storage->philosopher) * storage->input->number_of_philosophers);
	if (storage->philosopher == 0)
		return (ft_putstr_fd("Error with malloc\n", 1));
	storage->arr_of_forks = malloc(sizeof(*storage->arr_of_forks) * storage->input->number_of_philosophers);
	if (storage->arr_of_forks == 0)
		return (ft_putstr_fd("Error with malloc\n", 1));
	pthread_mutex_init(&storage->someone_dead, 0);
	pthread_mutex_init(&storage->someone_write, 0);

	int i = 0;

	while (i < storage->input->number_of_philosophers)
	{
		storage->philosopher[i].stor = storage;
		storage->philosopher[i].is_eating = 0;
		storage->philosopher[i].position = i;
		storage->philosopher[i].how_many_times_eat = 0;
		storage->philosopher[i].left_fork = i;
		storage->philosopher[i].right_fork = (i + 1) % storage->input->number_of_philosophers;
		pthread_mutex_init(&storage->philosopher[i].mutex_philo, 0);
		pthread_mutex_init(&storage->philosopher[i].eating, 0);
		//pthread_mutex_lock(&storage->philosopher[i].eating); // нужно ли?
		pthread_mutex_init(&storage->arr_of_forks[i], 0);
		i++;
	}

	int		j;
	void	*philosopher;

	j = 0;
	while (j < storage->input->number_of_philosophers)
	{
		if (pthread_create(&storage->philosopher[j].pthr, 0, life_cicl, (void*)&storage->philosopher[j]) != 0)
			return (ft_putstr_fd("Can't create thread\n", 1));
		pthread_join(storage->philosopher[j].pthr, 0);
	}
	return (0);
}
