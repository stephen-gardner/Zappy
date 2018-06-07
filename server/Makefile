#!/usr/bin/make -f

################################################################################
# SETTINGS                                                                     #
################################################################################

NAME = server
CC = gcc
CFLAGS += -Wall -Werror -Wextra -Wimplicit -g -fsanitize=address
INC = -I inc
SRC_DIR = src
OBJ_DIR = obj

# [ BASE ]

SRC = \
	error\
	main\
	opt\
	read\
	sockets\
	teams\
	util\
	write

# [ COMMANDS ]

SRC += \
	cmd/cmd\
	cmd/cmd_connect_nbr\
	cmd/cmd_fork\
	cmd/cmd_inventory\
	cmd/cmd_move

OBJ = $(patsubst %, $(OBJ_DIR)/%.o, $(SRC))

################################################################################
# COLORS                                                                       #
################################################################################

COLSIZE = 50
NC = \033[0m
GREEN = \033[1;32m
RED = \033[1;31m
YELLOW = \033[1;33m

################################################################################
# RULES                                                                        #
################################################################################

all: $(NAME)

$(NAME): $(OBJ)
	@printf "$(YELLOW)%-$(COLSIZE)s$(NC)" "Building $@... "
	@$(CC) $(CFLAGS) $(OBJ) -o $@
	@echo "$(GREEN)DONE$(NC)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo " > Compiling $<..."
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(RED)Object files removed$(NC)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)$(NAME) removed$(NC)"

re: fclean all