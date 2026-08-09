NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -O0 
THREAD_FLAGS = -lpthread

SRC = main.c monitor.c\
      check.c coder_mutex.c coder_operations.c\
      errors_handeling.c coder_operations2.c\
      helper_functions.c coder_readmutex.c\
      helper_functions2.c helper_functions3.c\
      simalation_creation.c simulation_start.c\
      scheduler_operations.c scheduler_operations2.c\
      scheduler_operations3.c


OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(THREAD_FLAGS) $(OBJ) -o $(NAME)

%.o: %.
	$(CC) $(THREAD_FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
