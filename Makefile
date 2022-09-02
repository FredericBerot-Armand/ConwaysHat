NAME = conways-hat.exe

CXXFLAGS = -Wall -Werror -Wextra -o3

INC = -I include

LIB = 

SRC = src/main.cpp\
	  src/FlashMobSimulator.cpp


OBJ = $(SRC:src/%.cpp=obj/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(INC) $(LIB) -o $(NAME) $^

obj/%.o: src/%.cpp
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re

