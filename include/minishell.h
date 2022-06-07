/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 15:51:33 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/08 00:57:43 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../library/libft/inc/libft.h"
# include <stdio.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>

# define TOKEN_RESERVED "|;><"
# define TOKEN_SPACE " \r\v\f\n\t"
# define TOKEN_QUOTE "\"'"
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

typedef struct s_lexer {
	char					*pa_str;
	int						read_pos;
	struct s_token_lst		*head;
} t_lexer;

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
char			*gnl_strjoin(char *s1, char *s2);
t_token_lst		*ft_init_token(char *pa_str, enum e_token_type token_type);
void			ft_read_char(t_lexer *out_lexer);
char			ft_get_char(t_lexer *out_lexer);
char			ft_peek_char(t_lexer *out_lexer);
void			ft_skip_space(t_lexer *out_lexer);
int				ft_get_pos(t_lexer *lexer);
void			ft_add_token(t_lexer *out_lexer, t_token_lst *new_token);
void			ft_tokenization_reserved(t_lexer *out_lexer);
void			ft_tokenization_argument(t_lexer *out_lexer);
void			ft_tokenization_quote(t_lexer *out_lexer, char *pa_str, char quote);
char			*ft_get_env(t_lexer *out_lexer);
char			*ft_dquote(t_lexer *out_lexer, char *pa_str);
char			*ft_squote(t_lexer *out_lexer, char *pa_str);
void			ft_tokenization(t_lexer *out_lexer);

/* src/parser/tokenizer/lexer.c */
t_lexer			*ft_init_lexer(char *str);

/*
 * #########################################################
 * #                                                       #
 * #                       src/parser/                     #
 * #                                                       #
 * #########################################################
 */
/* src/parser/parser.c */
t_argument			*ft_init_argument(void);
int					ft_argument_size(t_token_lst *cur_token);
void				ft_add_argument(t_argument **head, t_argument *argument);
t_token_lst			*ft_read_token(t_token_lst *cur_token, t_argument *argument, int index);
t_token_lst			*ft_read_token_only_type_argument(t_token_lst *cur_token, t_argument *argument, int index);
t_argument			*ft_command_to_argument(char *command);

#endif
