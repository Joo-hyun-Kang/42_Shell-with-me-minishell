# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/26 01:53:57 by kanghyki          #+#    #+#              #
#    Updated: 2022/05/25 09:52:40 by kanghyki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

COMPILE_COLOR	=	"\033[1;34m"
LIB_COLOR		=	"\033[1;32m"
DEL_COLOR		=	"\033[1;31m"
END				=	"\033[0;0m"
CLEAR			=	"\x1b[1A\x1b[M"

NAME	=	libft.a
INC		=	-I ./inc
SRCS	=	ft_bzero.c ft_atoi.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c \
			ft_strlen.c ft_strncmp.c ft_strchr.c ft_tolower.c ft_toupper.c ft_strrchr.c ft_memset.c \
			ft_strdup.c ft_memcmp.c ft_memcpy.c ft_memchr.c ft_calloc.c ft_strdup.c ft_strlcpy.c \
			ft_strlcat.c ft_strnstr.c ft_memmove.c \
			ft_itoa.c ft_split.c ft_strjoin.c ft_putchar_fd.c ft_putnbr_fd.c ft_substr.c ft_strtrim.c \
			ft_striteri.c ft_putendl_fd.c ft_putstr_fd.c ft_strmapi.c

OBJS	= $(SRCS:.c=.o)

BONUS	=   ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c ft_lstdelone.c ft_lstiter.c \
			ft_lstlast.c ft_lstmap.c ft_lstnew.c ft_lstsize.c

BONUS_OBJS	= $(BONUS:.c=.o)

RM		= rm -f
LIB_C	= ar -rcs
CC		= gcc
CFLAGS	= -Wall -Wextra -Werror

ifdef BONUS_FLAG
	OBJ = $(OBJS) $(BONUS_OBJS)
else
	OBJ = $(OBJS)
endif

%.o: %.c
	@echo $(COMPILE_COLOR) "Compiling...\t$<" $(END)
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(NAME): $(OBJ)
	@echo $(COMPILE_COLOR) "Building...\t$@" $(END)
	@$(LIB_C) $(NAME) $(OBJ)
	@echo $(LIB_COLOR) "Done" $(END)

all: $(NAME)

bonus:
	@make BONUS_FLAG=1 all

clean:
	@echo $(DEL_COLOR) "clean" $(END)
	@$(RM) $(OBJS) $(BONUS_OBJS)

fclean:
	@echo $(DEL_COLOR) "fclean" $(END)
	@$(RM) $(OBJS) $(BONUS_OBJS)
	@$(RM) $(NAME)

re:
	@make fclean
	@make all

.PHONY: all bonus clean fclean re
