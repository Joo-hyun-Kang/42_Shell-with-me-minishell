# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/01 00:54:06 by kanghyki          #+#    #+#              #
#    Updated: 2022/06/03 14:34:44 by kanghyki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#----------------[ COLOR ]------------------
COMPILE_COLOR	=	"\033[1;34m"
LIB_COLOR		=	"\033[1;32m"
DEL_COLOR		=	"\033[1;31m"
END				=	"\033[0;0m"
CLEAR			=	"\x1b[1A\x1b[M"
#--------------[ TOKEN ]----------------
NAME			=	libtoken.a
INC_DIR			=	inc
SRC_DIR			=	src
SRC				=	token.c\
					token_utils.c\
					lexer.c

SRCS			=	$(addprefix $(SRC_DIR)/, $(SRC))
OBJS			=	$(SRC:.c=.o)
#-----------------[ CMD ]-------------------
CC				=	gcc
CFLAGS			=	-Wall -Wextra -Werror
AR				=	ar -rcs
RM				=	rm -rf

all: $(NAME)

%.o: %.c
	@echo $(COMPILE_COLOR) "Compiling...\t$<" $(END)
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@echo $(COMPILE_COLOR) "Building...\t$@" $(END)
	@$(AR) $(NAME) $(OBJS)
	@echo $(LIB_COLOR) "Done" $(END)

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

clean:
	@$(RM) $(OBJS)
	@echo $(DEL_COLOR) "clean" $(END)

fclean:
	@$(RM) $(OBJS) $(NAME)
	@echo $(DEL_COLOR) "fclean" $(END)

re:
	@make fclean
	@make all

.PHONY: all clean fclean re

