# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jokang <jokang@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/01 00:54:06 by kanghyki          #+#    #+#              #
#    Updated: 2022/06/29 21:50:44 by jokang           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#----------------[ COLOR ]------------------
COMPILE_COLOR	=	"\033[1;34m"
LIB_COLOR		=	"\033[1;32m"
DEL_COLOR		=	"\033[1;31m"
END				=	"\033[0;0m"
CLEAR			=	"\x1b[1A\x1b[M"
#---------------[ libft ]-------------------
LIBFT			=	$(LIBFT_DIR)/libft.a
LIBFT_DIR		=	library/libft
LIBFT_INC		=	$(LIBFT_DIR)/inc
#---------------[ libft ]-------------------
GNL_DIR			=	library/get_next_line
GNL_SRC			=	get_next_line.c\
					get_next_line_utils.c
GNL_INC			=	$(GNL_DIR)/inc
#--------------[ SIGNAL ]-----------------
SIG_DIR			=	src/signal
SIG_SRC			=	signal.c
#--------------[ PROMPT ]-----------------
PRPT_DIR		=	src/prompt
PRPT_SRC		=	ft_echo.c\
					ft_exit.c\
					ft_readline.c
#--------------[ TOKENIZER ]-----------------
TOKEN_DIR		=	src/parser/tokenizer
TOKEN_SRC		=	tokenizer.c\
					tokenizer_utils.c\
					tokenizer_arg.c\
					lexer.c\
					lexer_utils.c\
					lexer_extra_utils.c
#--------------[ PARSER ]----------------
PARSER_DIR		=	src/parser
PARSER_SRC		=	parser.c\
					parser_utils.c\
					extra_pipe.c\
					heredoc.c\
					heredoc_utils.c\
					align_arg.c\
					ft_merge_str.c
#--------------[ MALLOC ]----------------
MALLOC_DIR		=	src/malloc
MALLOC_SRC		=	ft_strdup.c\
					ft_strndup.c
#--------------[ SAFE ]----------------
SAFE_DIR		=	src/safe
SAFE_SRC		=	malloc_safe.c\
					fork_safe.c\
					pipe_safe.c
#--------------[ ENV ]----------------
ENV_DIR			=	src/environment
ENV_SRC			=	env_utils.c\
					env_cpy.c
#--------------[ ENV_BST ]----------------
ENVBST_DIR		=	src/environment/environment_bst
ENVBST_SRC		=	env_bst_init.c\
					env_bst_insert.c\
					env_bst_search.c\
					env_bst_delete.c
#--------------[ EXECUTE ]----------------
EXECUTE_DIR		=	src/execute_cmd
EXECUTE_SRC		=	cmd.c\
					cmd_single.c\
					execute.c\
					execute_path.c\
					execute_except.c\
					pipe.c\
					pipe_utils.c\
					redir.c\
					redir_set.c\
					redir_set_utils.c\
					redir_sort.c\
					redir_utils.c\
					arraylist.c\
					arraylist_utils.c\
					ft_error.c\
					ft_error_sec.c
#--------------[ BUILTIN ]----------------
BUILTIN_DIR		=	src/builtin
BUILTIN_SRC		=	builtin.c\
					export.c\
					env.c\
					unset.c\
					echo.c\
					exit.c\
					pwd.c\
					cd.c
#--------------[ MINISHELL ]----------------
NAME			=	minishell	
OBJ_DIR			=	objects
SRC_DIR			=	src
SRC				=	main.c

OBJS			=	$(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))\
					$(addprefix $(OBJ_DIR)/, $(GNL_SRC:.c=.o))\
					$(addprefix $(OBJ_DIR)/, $(PARSER_SRC:.c=.o))\
					$(addprefix $(OBJ_DIR)/, $(TOKEN_SRC:.c=.o))\
					$(addprefix $(OBJ_DIR)/, $(ENV_SRC:.c=.o))\
					$(addprefix $(OBJ_DIR)/, $(ENVBST_SRC:.c=.o))\
					$(addprefix $(OBJ_DIR)/, $(MALLOC_SRC:.c=.o))\
					$(addprefix $(OBJ_DIR)/, $(SAFE_SRC:.c=.o))\
					$(addprefix $(OBJ_DIR)/, $(SIG_SRC:.c=.o))\
					$(addprefix $(OBJ_DIR)/, $(EXECUTE_SRC:.c=.o))\
					$(addprefix $(OBJ_DIR)/, $(PRPT_SRC:.c=.o))\
					$(addprefix $(OBJ_DIR)/, $(BUILTIN_SRC:.c=.o))\
#-----------------[ CMD ]-------------------
CC				=	gcc
CFLAGS			=	-Wall -Wextra -Werror
CPPFLAGS		=	-I include\
					-I $(LIBFT_DIR)\
					-I $(GNL_DIR)\
					-I/Users/jokang/.brew/opt/readline/include
LDLIBS			=	-l ft -L $(LIBFT_DIR)\
					-l readline -L/Users/jokang/.brew/opt/readline/lib
AR				=	ar -rcs
RM				=	rm -rf

vpath %.c $(SRC_DIR) $(GNL_DIR) $(TOKEN_DIR) $(PARSER_DIR) $(ENV_DIR) $(ENVBST_DIR) $(MALLOC_DIR) $(SAFE_DIR) $(SIG_DIR) $(EXECUTE_DIR) $(PRPT_DIR) $(BUILTIN_DIR)

all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	@echo $(COMPILE_COLOR) "Compiling...\t$<" $(END)
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@ -g3

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
