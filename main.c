#include "philosophers.h"

void *lunch_counter(void *storage_input)
{
	t_storage *storage;
	int i;
	int count;

	storage = (t_storage*)storage_input;
	count = 0;
	while (count < storage->input->number_of_times_each_philosopher_must_eat)
	{
		i = 0;
		while (i < storage->input->number_of_philosophers)
			pthread_mutex_lock(&storage->philosopher[i++].eating);
		count++;
	}
	pthread_mutex_lock(&storage->someone_write);
	
}

int create_thread(t_storage *storage)
{
	pthread_t	thread;
	int i;

	storage->time = get_time();
	if (storage->input->number_of_times_each_philosopher_must_eat > 0)
	{
		if (pthread_create(&thread, 0, &m, (void*)storage) != 0)
			return (ft_putstr_fd("Can't create thread\n", 1));
		pthread_detach(thread);
	}
}

int	main(int argc, char **argv)
{
	t_input_argv	input;
	t_storage		storage;

	storage.input = &input;
	if (argc < 5 || argc > 6)
		return (ft_putstr_fd("Wrong number of arguments\n", 1));
	if (initialization(&storage, argc, argv))
		return (ft_exit(&storage));
	if (create_thread(&storage))
		return (ft_exit(&storage));
}
