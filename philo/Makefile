NAME = philo

OBJDIR	=	objs/
SRCS = main.c ft_exit.c initialization.c utils.c check_is_dead.c launch_thread.c
OBJ 	=	$(SRCS:.c=.o)
OBJS	= 	$(addprefix $(OBJDIR), $(OBJ))

CFLAGS  = -Wall -Wextra -Werror -I. -O3
all:	$(NAME)

$(NAME):	$(OBJS)
	$(CC) $(CFLAGS) -O3 $(OBJS)  -o $(NAME) -lpthread #-fsanitize=address

$(OBJDIR)%.o: %.c Makefile philosophers.h
	mkdir -p $(OBJDIR)
	gcc -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean:	clean
	rm -rf $(NAME)

re:	fclean $(NAME)

.PHONY:	all clean fclean re

