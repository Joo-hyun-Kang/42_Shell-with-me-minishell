/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 17:18:03 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/06 15:23:02 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H 
# define TOKEN_H 

# include <stdlib.h>
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

/* token.c */
t_token_lst			*ft_new_token(enum e_token_type type, char *str);
t_token_lst	 		*ft_create_next_token(t_lexer *l);
void				ft_read_char(t_lexer *l);
char				ft_lookup_char(t_lexer *l);
char				ft_lookup_next_char(t_lexer *l);
void				ft_skip_whitespace(t_lexer *l);
char				*ft_quote(char *str, t_lexer *l, char quote);
char				*ft_read_argument(t_lexer *l);

/* token_utils.c */
char				*ft_strjoin(char *s1, char *s2);
int					ft_strlen(const char *str);
char				*ft_strndup(const char *str, int n);
char				*ft_strchr(const char *str, int c);

/* lexer.c */
t_lexer				*ft_init_lexer(char *str);
void				ft_analyze_string(t_lexer *out_l);

#endif
