/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 15:51:33 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/09 04:39:49 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../library/libft/inc/libft.h"
# include <stdio.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>

# define METACHAR "|;><"
# define WHITE_SPACE " \r\v\f\n\t"
//# define QUOTE "\"'"

# define BANNER "\n\n\n\
     S  H  E  L  L          W  I  T  H          M  E\n\
     -  -  -  -  -          -  -  -  -          -  -\n\
                                                    \n\
                                   Jokang   Kanghyki\n\
                                   ------ & --------\n\n"

enum e_token_type {
	ARGUMENT,
	PIPE,					// |
	SEMICOLON,				// ;
	/* Redirections */
	LT,						// >
	DLT,					// >>
	GT,						// <
	DGT,					// <<
	EOL
};

typedef struct s_token_lst {
	char					*pa_str;
	enum e_token_type		token_type;
	struct s_token_lst		*next;
} t_token_lst;

typedef struct s_argument {
	enum e_token_type		next_token_type;
	char					**pa_argument;
	struct s_argument		*next;
} t_argument;

/*
 * #########################################################
 * #                                                       #
 * #                src/parser/tokenizer/                  #
 * #                                                       #
 * #########################################################
 */
/* src/parser/tokenizer/token.c */
char			*ft_strndup(const char *src, size_t n);
t_token_lst		*ft_init_token(char *pa_str, enum e_token_type token_type);
void			ft_add_token(t_token_lst **head, t_token_lst *new_token);
t_token_lst		*ft_create_token_meta_char(char **str);
t_token_lst		*ft_create_token_argument(char **str);
t_token_lst		*ft_tokenization(char *str);

/*
 * #########################################################
 * #                                                       #
 * #                       src/parser/                     #
 * #                                                       #
 * #########################################################
 */
/* src/parser/parser.c */
t_argument		*ft_init_argument(void);
char			**ft_malloc_pa_argument(t_token_lst *cur_token);
void			ft_add_argument(t_argument **head, t_argument *argument);
t_token_lst		*ft_read_token(t_token_lst *cur_token, t_argument *argument, int index);
t_token_lst		*ft_read_token_state_only_argument(t_token_lst *cur_token, t_argument *argument, int index);
t_argument		*ft_str_to_argument(char *str);
void			ft_delete_token(t_token_lst *token);
void			ft_delete_argument(t_argument *arg);

#endif
