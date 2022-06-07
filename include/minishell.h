/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 15:51:33 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/07 16:39:59 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../library/libft/inc/libft.h"
# include <stdbool.h>
#include <stdio.h>

# define TOKEN_RESERVED "|;><"
# define TOKEN_SPACE " \r\v\f\n\t"
# define TOKEN_QUOTE "\"'"

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
	enum e_token_type	next_token_type;
	char				**pa_argument;
	char				*use_only_redirection;
	struct s_argument	*next;
} t_argument;

typedef struct s_lexer {
	char					*pa_str;
	int						read_pos;
	struct s_token_lst		*head;
} t_lexer;

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
char				**ft_init_pa_argument(int size);
int					ft_get_pa_argument_size(t_lexer *lexer);
void				ft_read_token(t_lexer *lexer);
t_token_lst	*ft_lookup_token(t_lexer *lexer);
t_argument			*ft_create_argument(t_lexer *lexer);
t_argument			*ft_add_argument(t_argument *head, t_argument *add);
t_argument			*ft_parse_token(t_lexer *lexer);
/* API */
t_argument			*ft_parse_command(char *command);

#endif
