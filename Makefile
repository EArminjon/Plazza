NAME	= 	plazza

CC	=	g++ -lpthread

RM	=	rm -f

SRCS	=	srcs/main.cpp			\
		srcs/console.cpp		\
		srcs/console_prompt.cpp		\
		srcs/console_checker.cpp	\
		srcs/console_child.cpp		\
		srcs/console_file.cpp		\
		srcs/slave.cpp			\
		srcs/parser.cpp			\
		srcs/threadPool.cpp		\
		srcs/log.cpp			\
		srcs/communication.cpp		\
		srcs/serializer.cpp

OBJS	=	$(SRCS:.cpp=.o)

CPPFLAGS	=	-I include

CPPFLAGS	+=	-W -Wall -Wextra -g3

LDFLAGS =

.cpp.o:
		@$(CC) $(CPPFLAGS) $(LDFLAGS) -c $< -o $@ && \
		printf "[\033[1;32mcompiled\033[0m] % 34s\n" $< | tr ' ' '.' || \
		printf "[\033[1;31mfailed\033[0m] % 36s\n" $< | tr ' ' '.'

all: 		$(NAME)

$(NAME):	$(OBJS)
		@$(CC) $(OBJS) -o $(NAME) $(LDFLAGS) && \
		printf "[\033[1;36mbuilt\033[0m] % 37s\n" $(NAME) | tr ' ' '.' || \
		printf "[\033[1;31mfailed\033[0m] % 36s\n" $(NAME) | tr ' ' '.'


clean:
		@$(RM) $(OBJS) && \
		printf "[\033[1;31mdeleted\033[0m] % 35s\n" $(OBJS) | tr ' ' '.' || \
		printf "[\033[1;31mdeleted\033[0m] % 35s\n" $(OBJS) | tr ' ' '.'

fclean: 	clean
		@$(RM) $(NAME) && \
		printf "[\033[1;31mdeleted\033[0m] % 35s\n" $(NAME) | tr ' ' '.' || \
		printf "[\033[1;31mdeleted\033[0m] % 35s\n" $(NAME) | tr ' ' '.'

re:		fclean all

.PHONY: 	all clean fclean re
