#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_input_argv	input;
	t_storage		storage;

	storage.input = &input;
	if (argc < 5 || argc > 6)
		return (1 + ft_putstr_fd("Wrong number of arguments\n", 1));
	if (ft_atoi(argv[1]) < 0 || ft_atoi(argv[2]) < 0
		|| ft_atoi(argv[3]) < 0 || ft_atoi(argv[4]) < 0)
		return (1 + ft_putstr_fd("One of arguments is negative\n", 1));
	if (initialization(&storage, argc, argv))
		return (ft_exit(&storage));
	pthread_mutex_destroy(storage.arr_of_forks);
	pthread_mutex_destroy(&storage.print);
	pthread_mutex_destroy(&storage.someone_dead);
	free(storage.arr_of_forks);
	free(storage.philosopher);
	return (0);
}
