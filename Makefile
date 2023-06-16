#Color scheme
COLOUR_GREEN=\033[0;32m
COLOUR_RED=\033[0;31m
COLOUR_BLUE=\033[0;34m
COLOUR_END=\033[0m

NAME = ircserv
CC := c++
#Flags
COMPILE_FLAGS = -std=c++98 -Wall -Wextra -Werror

#Dirs
OBJS_DIR = obj/
CORE_DIR = srcs/Core/
SERVER_DIR = srcs/Server/
CLIENT_DIR = srcs/Client/

CORE = main.cpp
CLIENT = Client.cpp
SERVER = Server.cpp

ALL_SRCS = $(CLIENT) $(SRC) $(CORE) $(SERVER)
OBJ_FILES = $(ALL_SRCS:.cpp=.o)
OBJS = $(patsubst %, $(OBJS_DIR)%, $(ALL_SRCS:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJS_DIR) $(OBJS)
	@$(CC) -o $(NAME) $(COMPILE_FLAGS) $(OBJS)
	@echo "$(COLOUR_GREEN)$@ created$(COLOUR_END)"

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)
	@echo "$(COLOUR_BLUE)object directory created$(COLOUR_END)"

$(OBJS_DIR)%.o: $(CORE_DIR)%.cpp
	@cc $(FLAGS) -c $< -o $@
	@echo "$(COLOUR_BLUE)$@ created$(COLOUR_END)"

$(OBJS_DIR)%.o: $(CLIENT_DIR)%.cpp
	@cc $(FLAGS) -c $< -o $@
	@echo "$(COLOUR_BLUE)$@ created$(COLOUR_END)"

$(OBJS_DIR)%.o: $(SERVER_DIR)%.cpp
	@cc $(FLAGS) -c $< -o $@
	@echo "$(COLOUR_BLUE)$@ created$(COLOUR_END)"

clean:
	@rm -f $(OBJS)
	@rm -rf $(OBJS_DIR)
	@echo "$(COLOUR_BLUE)object directory cleaned$(COLOUR_END)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(COLOUR_RED)$(NAME) removed$(COLOUR_END)"

re: fclean all

.PHONY: all clean fclean re