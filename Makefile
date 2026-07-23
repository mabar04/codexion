NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror
THREAD_FLAGS = -pthread

SRC = main.c \
      check.c \
      errors_handeling.c \
      helper_functions.c \
      parsing.c \
      simalation_creation.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(THREAD_FLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(THREAD_FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re