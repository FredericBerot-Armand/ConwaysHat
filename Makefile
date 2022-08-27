NAME = conways-hat

FLAGS = -Wall -Werror -Wextra -o2

CC = g++ $(FLAGS)

INC = -I include

LIB = 


SRC = src/main.cpp\
	src/FlashMob.cpp


OBJ = $(SRC:src/%.cpp=obj/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(INC) $(LIB) -o $(NAME) $^

obj/%.o: src/%.cpp
	@mkdir -p obj
	$(CC) $(INC) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re

