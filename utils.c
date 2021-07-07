#include "philosophers.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != 0)
	{
		i++;
	}
	return (i);
}

long	ft_atoi(const char *nptr)
{
	int		i;
	long	j;
	int		minus;

	i = 0;
	j = 0;
	minus = 1;
	while (nptr[i] == ' ' || nptr[i] == '	' || nptr[i] == '\n'
		|| nptr[i] == '\t' || nptr[i] == '\f'
		|| nptr[i] == '\r' || nptr[i] == '\v')
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			minus = (-1);
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		j = (j * 10) + (nptr[i] - 48);
		i++;
	}
	return (j * minus);
}

int	ft_putstr_fd(char *s, int fd)
{
	if (s)
		if (write(fd, s, ft_strlen(s)) != 0)
			return (0);
	return (fd);
}

unsigned long	my_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * (unsigned long)1000) + (time.tv_usec / 1000));
}
