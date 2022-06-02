# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/01 00:54:06 by kanghyki          #+#    #+#              #
#    Updated: 2022/06/02 12:34:59 by kanghyki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#----------------[ COLOR ]------------------
COMPILE_COLOR	=	"\033[1;34m"
LIB_COLOR		=	"\033[1;32m"
DEL_COLOR		=	"\033[1;31m"
END				=	"\033[0;0m"
CLEAR			=	"\x1b[1A\x1b[M"
#--------------[ MINISHELL ]----------------
NAME			=	minishell	
INC_DIR			=	inc
OBJ_DIR			=	obj
SRC_DIR			=	src
SRC				=	main.c

SRCS			=	$(addprefix $(SRC_DIR)/, $(SRC))
OBJS			=	$(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))
#-----------------[ CMD ]-------------------
CC				=	gcc
CFLAGS			=	-Wall -Wextra -Werror
CPPFLAGS		=	-I $(INC)
LDFLAGS			=	-lncurses
AR				=	ar -rcs
RM				=	rm -rf

vpath %.c $(SRC_DIR)

all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	@echo $(COMPILE_COLOR) "Compiling...\t$<" $(END)
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ_DIR) $(OBJS)
	@echo $(COMPILE_COLOR) "Building...\t$@" $(END)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)
	@echo $(LIB_COLOR) "Done" $(END)

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

clean:
	@$(RM) $(OBJ_DIR)
	@echo $(DEL_COLOR) "clean" $(END)

fclean:
	@$(RM) $(OBJ_DIR) $(NAME)
	@echo $(DEL_COLOR) "fclean" $(END)

re:
	@make fclean
	@make all

.PHONY: all clean fclean re

