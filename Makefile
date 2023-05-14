SRC=    main.c print.c utils.c\

OBJS			= $(SRC:.c=.o)

NAME			= ft_nm

CFLAGS			=  -g

RM				= rm -f

CC				= gcc

%.o : %.c
				$(CC) $(CFLAGS) -c $< -o $@

$(NAME):		$(OBJS)
				$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -L.

all:			$(NAME)

clean:
				$(RM) $(OBJS) 

fclean:			clean
				$(RM) $(NAME)

re:				fclean all

.PHONY:			all clean fclean c.o re 