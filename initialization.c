#include "philosophers.h"

void *check_is_dead(void *ptr)
{
	t_philosopher *philo;

	philo = (t_philosopher*)ptr;
	while (philo->stor->is_dead == 0)
	{
		usleep(100);
		pthread_mutex_lock(&philo->eating);
		if (my_time() - philo->last_eat > (unsigned int)philo->stor->input->time_to_die && philo->stor->is_dead == 0)
		{
			philo->stor->is_dead = 1;
			pthread_mutex_unlock(&philo->eating);
			pthread_mutex_lock(&philo->stor->someone_dead);
			printf("%lu ms %d died\n", (my_time() - philo->stor->time_start), philo->position);
			return (0);
		}
		else if (philo->how_many_times_eat >= philo->how_many_times_must_eat
			&& philo->how_many_times_must_eat != -1 && philo->stor->is_dead == 0)
			return (0);
		pthread_mutex_unlock(&philo->eating);
	}
	return (0);
}

void			printf_pro(t_philosopher *p, char *str, char *smile)
{
	if (p->stor->is_dead == 0 && p->stor->total_number_of_meals != 0)
	{
		pthread_mutex_lock(&p->stor->print);
		if (!p->stor->is_dead && p->stor->total_number_of_meals != 0)
			printf("%lu ms %d %s %s\n", (my_time() - p->stor->time_start),
			p->position, str, smile);
		pthread_mutex_unlock(&p->stor->print);
	}
}

void			my_sleep(int time, t_storage *all)
{
	unsigned long end;

	end = my_time() + time;
	while (my_time() < end && !all->is_dead)
		usleep(time);
}

void eating_spaghetti(t_philosopher *p)
{
	pthread_mutex_lock(&p->stor->arr_of_forks[p->right_fork]);
	printf_pro(p, "has taken a right fork", FORK);
	pthread_mutex_lock(&p->stor->arr_of_forks[p->left_fork]);
	printf_pro(p, "has taken a left fork", FORK);
	pthread_mutex_lock(&p->eating);
	p->last_eat = my_time();
	printf_pro(p, "is eating", FORK_AND_PLATE);
	p->how_many_times_eat++;
	if (p->stor->total_number_of_meals  > 0)
		p->stor->total_number_of_meals--;
	my_sleep(p->stor->input->time_to_eat, p->stor);
	pthread_mutex_unlock(&p->eating);
	pthread_mutex_unlock(&p->stor->arr_of_forks[p->right_fork]);
	pthread_mutex_unlock(&p->stor->arr_of_forks[p->left_fork]);
	printf_pro(p, "is sleeping", SLEEP);
	my_sleep(p->stor->input->time_to_sleep, p->stor);
	printf_pro(p, "is thinking", THINK);
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
	if (philo->position % 2 == 0 && philo->stor->input->time_to_eat > 1)
		my_sleep(philo->stor->input->time_to_eat * 0.9, philo->stor);
	while (philo->stor->is_dead == 0 && (philo->how_many_times_must_eat == -1 ||
		philo->how_many_times_must_eat > philo->how_many_times_eat))
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
		if (ft_atoi(argv[5]) <= 0)
			return(ft_putstr_fd("Number_of_times_each_philosopher_must_eat must be positive\n", 1));
		storage->input->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	}
	else
		storage->input->number_of_times_each_philosopher_must_eat = -1;
	storage->philosopher = 0;
	storage->arr_of_forks = 0;
	storage->is_dead = 0;
	if (storage->input->number_of_times_each_philosopher_must_eat != -1)
		storage->total_number_of_meals = storage->input->number_of_times_each_philosopher_must_eat * storage->input->number_of_philosophers;
	else
		storage->total_number_of_meals = -1;
	if (storage->input->number_of_philosophers < 2)
		return (ft_putstr_fd("Not enough philosopfhers\n", 1));
	if (storage->input->time_to_die < 60 || storage->input->time_to_eat < 60 || storage->input->time_to_sleep < 60)
		return (ft_putstr_fd("So little time for eat or die or sleep\n", 1));
	if (storage->input->number_of_philosophers > 200)
		return (ft_putstr_fd("To many philosophers\n", 1));

	storage->philosopher = malloc(sizeof(*storage->philosopher) * storage->input->number_of_philosophers);
	if (storage->philosopher == 0)
		return (ft_putstr_fd("Error with malloc\n", 1));
	storage->arr_of_forks = malloc(sizeof(*storage->arr_of_forks) * storage->input->number_of_philosophers);
	if (storage->arr_of_forks == 0)
		return (ft_putstr_fd("Error with malloc\n", 1));
	pthread_mutex_init(&storage->someone_dead, 0);
	pthread_mutex_init(&storage->print, 0);

	storage->time_start = my_time();

	int i = 0;

	while (i < storage->input->number_of_philosophers)
	{
		storage->philosopher[i].stor = storage;
		storage->philosopher[i].is_eating = 0;
		storage->philosopher[i].position = i + 1;
		storage->philosopher[i].how_many_times_eat = 0;
		storage->philosopher[i].how_many_times_must_eat = storage->input->number_of_times_each_philosopher_must_eat;
		storage->philosopher[i].left_fork = i;
		storage->philosopher[i].right_fork = (i + 1) % storage->input->number_of_philosophers;
		pthread_mutex_init(&storage->philosopher[i].mutex_philo, 0);
		pthread_mutex_init(&storage->philosopher[i].eating, 0);
		//pthread_mutex_lock(&storage->philosopher[i].eating); // нужно ли?
		pthread_mutex_init(&storage->arr_of_forks[i], 0);
		i++;
	}

	int		j;

	j = 0;
	while (j < storage->input->number_of_philosophers)
	{
		if (pthread_create(&storage->philosopher[j].pthr, 0, life_cicl, (void*)&storage->philosopher[j]) != 0)
			return (ft_putstr_fd("Can't create thread\n", 1));
		j++;
	}
	j = 0;
	while(j < storage->input->number_of_philosophers)
	{
		pthread_join(storage->philosopher[j].pthr, 0);
		j++;
	}
	return (0);
}
