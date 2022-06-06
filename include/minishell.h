/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 15:51:33 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/06 15:54:51 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../library/libft/inc/libft.h"
# include <stdbool.h>

# define TOKEN_SEPARATOR "|;><"

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

typedef struct s_argument {
	enum e_token_type	next_token_type;
	char				**pa_argument;
} t_argument;

typedef struct s_token_lst {
	char					*pa_str_or_null;
	enum e_token_type		token_type;
	struct s_token_lst		*next;
} t_token_lst;

typedef struct s_lexer {
	char					*pa_str;
	int						read_pos;
	char					ch;
	struct s_token_lst		*head_token;
} t_lexer;

/* src/parser/tokenizer/token.c */
t_token_lst			*ft_new_token(enum e_token_type type, char *str);
t_token_lst	 		*ft_create_next_token(t_lexer *l);
void				ft_read_char(t_lexer *l);
char				ft_lookup_char(t_lexer *l);
char				ft_lookup_next_char(t_lexer *l);
void				ft_skip_whitespace(t_lexer *l);
char				*ft_quote(char *str, t_lexer *l, char quote);
char				*ft_read_argument(t_lexer *l);

/* src/parser/tokenizer/token_utils.c */
char				*ft_strndup(const char *str, int n);

/* src/parser/tokenizer/lexer.c */
t_lexer				*ft_init_lexer(char *str);
void				ft_analyze_string(t_lexer *out_l);

#endif
