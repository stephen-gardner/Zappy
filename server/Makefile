#!/usr/bin/make -f

################################################################################
# SETTINGS                                                                     #
################################################################################

NAME = server
CC = gcc
CFLAGS += -Wall -Werror -Wextra -Wimplicit -Ofast #-g -fsanitize=address
INC = -I inc
SRC_DIR = src
OBJ_DIR = obj

# [ BASE ]

SRC = \
	egg\
	gfx\
	main\
	map\
	opt\
	player\
	read\
	sockets\
	teams\
	util\
	write

# [ EVENTS: AI ]

SRC += \
	events/event\
	events/ai/event_broadcast\
	events/ai/event_connect_nbr\
	events/ai/event_fork\
	events/ai/event_incantation\
	events/ai/event_inventory\
	events/ai/event_kick\
	events/ai/event_move\
	events/ai/event_see\

# [ EVENTS: GFX ]

SRC += \
	events/gfx/event_bct\
	events/gfx/event_msz\
	events/gfx/event_sgt\
	events/gfx/event_sst\
	events/gfx/event_tna

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
