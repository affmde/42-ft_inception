#Color scheme
COLOUR_GREEN=\033[0;32m
COLOUR_RED=\033[0;31m
COLOUR_YELLOW=\033[0;33m
COLOUR_BLUE=\033[0;34m
COLOUR_END=\033[0m

NAME = ircserv
CC := c++
#Flags
COMPILE_FLAGS = -std=c++98 -Wall -Wextra -Werror

#Dirs
OBJS_DIR = obj/
SRCS_DIR = srcs/
COMMANDS_DIR = srcs/commands/

SRCS := main.cpp Client.cpp Server.cpp Parser.cpp Message.cpp Time.cpp Channel.cpp Command.cpp
COMMANDS := ACommand.cpp NICK.cpp JOIN.cpp PART.cpp
ALL_SRCS = $(SRCS) $(COMMANDS)


OBJ_FILES = $(SRCS:.cpp=.o) $(ALL_SRCS:.cpp=.o)
OBJS = $(patsubst %, $(OBJS_DIR)%, $(ALL_SRCS:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJS_DIR) $(OBJS)
	@$(CC) -o $(NAME) $(COMPILE_FLAGS) $(OBJS)
	@echo "$(COLOUR_GREEN)✓	$@ created$(COLOUR_END)"

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)
	@echo "$(COLOUR_BLUE)object directory created$(COLOUR_END)"

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp
	@cc $(FLAGS) -c $< -o $@
	@echo "$(COLOUR_GREEN)[OK]$(COLOUR_END)$(COLOUR_YELLOW)	$@ created$(COLOUR_END)"

$(OBJS_DIR)%.o: $(COMMANDS_DIR)%.cpp
	@cc $(FLAGS) -c $< -o $@
	@echo "$(COLOUR_GREEN)[OK]$(COLOUR_END)$(COLOUR_YELLOW)	$@ created$(COLOUR_END)"

clean:
	@rm -f $(OBJS)
	@rm -rf $(OBJS_DIR)
	@echo "$(COLOUR_BLUE)object directory cleaned$(COLOUR_END)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(COLOUR_RED)$(NAME) removed$(COLOUR_END)"

re: fclean all

.PHONY: all clean fclean re
