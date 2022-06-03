# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/01 00:54:06 by kanghyki          #+#    #+#              #
#    Updated: 2022/06/04 01:14:02 by kanghyki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#--------------[ TOKEN ]-----------------
TOKEN_DIR		=	token
TOKEN_SRC		=	token.c\
					token_utils.c\
					lexer.c
# TOKEN_SRCS		=	$(addprefix $(TOKEN_DIR)/, $(TOKEN_SRC))
#--------------[ PARSER ]----------------
NAME			=	libparser.a
SRC_DIR			=	.
SRC				=	parser.c

# SRCS			=	$(addprefix $(SRC_DIR)/, $(SRC))
#--------------[ OBJECTS ]---------------
OBJ_DIR			=	temporary_object_files
OBJS			=	$(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))\
					$(addprefix $(OBJ_DIR)/, $(TOKEN_SRC:.c=.o))
#--------------[ CMD ]-------------------
CC				=	gcc
CFLAGS			=	-Wall -Wextra -Werror -g
CPPFLAGS		=	-I $(SRC_DIR)\
					-I $(TOKEN_DIR)
AR				=	ar -rcs
RM				=	rm -rf

vpath %.c $(SRC_DIR) $(TOKEN_DIR)

all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ_DIR) $(OBJS)
	$(AR) $(NAME) $(OBJS)

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

clean:
	$(RM) $(OBJS) $(OBJ_DIR)

fclean:
	$(RM) $(OBJS) $(OBJ_DIR) $(NAME)

re:
	make fclean
	make all

.PHONY: all clean fclean re
