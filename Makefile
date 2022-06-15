# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/01 00:54:06 by kanghyki          #+#    #+#              #
#    Updated: 2022/06/15 19:03:24 by kanghyki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#----------------[ COLOR ]------------------
COMPILE_COLOR	=	"\033[1;34m"
LIB_COLOR		=	"\033[1;32m"
DEL_COLOR		=	"\033[1;31m"
END				=	"\033[0;0m"
CLEAR			=	"\x1b[1A\x1b[M"
#---------------[ libft ]-------------------
LIBFT			=	$(LIBFT_DIR)libft.a
LIBFT_DIR		=	library/libft/
LIBFT_INC		=	$(LIBFT_DIR)inc
#--------------[ SIGNAL ]-----------------
SIG_DIR			=	src/signal
SIG_SRC			=	signal.c
#--------------[ TOKENIZER ]-----------------
TOKEN_DIR		=	src/parser/tokenizer
TOKEN_SRC		=	tokenizer.c\
					tokenizer_core.c\
					tokenizer_utils.c
#--------------[ PARSER ]----------------
PARSER_DIR		=	src/parser
PARSER_SRC		=	parser.c\
					parser_utils.c\
					parser_common_utils.c\
					parser_free_utils.c\
					parser_additional.c
#--------------[ ENV ]----------------
ENV_DIR			=	src/environment
ENV_SRC			=	env.c\
					env_utils.c\
					env_utils_t.c\
					env_free_utils.c\
					env_execute.c\
					env_bst.c
#--------------[ EXCUSE ]----------------
EXCUSE_DIR		=	src/excuse_cmd
EXCUSE_SRC		=	cmd.c
#--------------[ MINISHELL ]----------------
NAME			=	minishell	
OBJ_DIR			=	objects
SRC_DIR			=	src
SRC				=	main.c

OBJS			=	$(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))\
					$(addprefix $(OBJ_DIR)/, $(PARSER_SRC:.c=.o))\
					$(addprefix $(OBJ_DIR)/, $(TOKEN_SRC:.c=.o))\
					$(addprefix $(OBJ_DIR)/, $(ENV_SRC:.c=.o))\
					$(addprefix $(OBJ_DIR)/, $(SIG_SRC:.c=.o))\
					$(addprefix $(OBJ_DIR)/, $(EXCUSE_SRC:.c=.o))
#-----------------[ CMD ]-------------------
CC				=	gcc
CFLAGS			=	-g # -Wall -Wextra -Werror
CPPFLAGS		=	-I include\
					-I $(EXCUSE_DIR)\
					-I /opt/homebrew/opt/readline/include
					#-I /Users/kanghyki/.brew/opt/readline/include
LDLIBS			=	-l ft -L $(LIBFT_DIR)\
					-l readline -L /opt/homebrew/opt/readline/lib
					#-l readline -L /Users/kanghyki/.brew/opt/readline/lib
AR				=	ar -rcs
RM				=	rm -rf

vpath %.c $(SRC_DIR) $(TOKEN_DIR) $(PARSER_DIR) $(ENV_DIR) $(SIG_DIR) $(EXCUSE_DIR)

all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	@echo $(COMPILE_COLOR) "Compiling...\t$<" $(END)
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(NAME): $(LIBFT) $(OBJ_DIR) $(OBJS)
	@echo $(COMPILE_COLOR) "Building...\t$@" $(END)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDLIBS)
	@echo $(LIB_COLOR) "Done" $(END)

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

$(LIBFT):
	@make -C $(LIBFT_DIR)

clean:
	@make clean -C $(LIBFT_DIR)
	@$(RM) $(OBJ_DIR)
	@echo $(DEL_COLOR) "clean" $(END)

fclean:
	@make fclean -C $(LIBFT_DIR)
	@$(RM) $(OBJ_DIR) $(NAME)
	@echo $(DEL_COLOR) "fclean" $(END)

re:
	@make fclean
	@make all

.PHONY: all clean fclean re

