#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_input_argv	input;
	t_storage		storage;

	storage.input = &input;
	if (argc < 5 || argc > 6)
		return (ft_putstr_fd("Wrong number of arguments\n", 1));
	if (initialization(&storage, argc, argv))
		return (ft_exit(&storage));
	pthread_mutex_destroy(storage.arr_of_forks);
	pthread_mutex_destroy(&storage.print);
	pthread_mutex_destroy(&storage.someone_dead);
	free(storage.arr_of_forks);
	free(storage.philosopher);
	return (0);
}
