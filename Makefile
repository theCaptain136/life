NAME	= life

CC	= gcc

RM	= rm -f

SRCS	=	./main.c \

OBJS	= $(SRCS:.c=.o)

CFLAGS = -I ./include/
CFLAGS += -Wall -Wextra -lm -L./lib -lmy -lcsfml-graphics -lcsfml-window -lcsfml-system -lcsfml-audio -lcsfml-network -g3

all: $(NAME)

$(NAME): $(OBJS)
	cd lib/my && make
	$(CC) $(OBJS) -o $(NAME) $(CFLAGS)

clean:
	$(RM) $(OBJS)

fclean:	clean
	$(RM) $(NAME)
	$(RM) lib/libmy.a
	$(RM) lib/my/libmy.a

re: fclean all

.PHONY: all clean fclean re
