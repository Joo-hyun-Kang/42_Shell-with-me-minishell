/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 17:18:03 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/04 03:24:30 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H 
# define TOKEN_H 

# include <stdlib.h>
# include <stdbool.h>

# define TOKEN_SET "|;><"

enum e_token_type {
	/* String */
	COMMAND,
	ARGUMENT,
	/*  */
	PIPE,					// |
	SEMICOLON,				// ;
	/* Redirections */
	LT,						// >
	DLT,					// >>
	GT,						// < (greater)
	DGT,					// <<
	EOL
};

// Token consisting of strings divided into meaningful units (string - tokenType).
typedef struct s_token_lst {
	char					*str;
	enum e_token_type		t_type;			// Token type
	struct s_token_lst		*next;
	struct s_token_lst		*prev;
} t_token_lst;

// Lexical analyzer(어휘 분석기): Lexical analyzer make token into input string.
typedef struct s_lexer_lst {
	char					*str;
	int						r_pos;			// Current read position.
	char					ch;				// Current read position character.
	struct s_token_lst		*head_token;
	struct s_cmd_lst		*next;
	struct s_cmd_lst		*prev;
} t_lexer_lst;

/* token.c */
t_token_lst			*ft_new_token(enum e_token_type type, char *str);
t_token_lst 		*ft_create_next_token(t_lexer_lst *l, int *out_seperate);
void				ft_read_char(t_lexer_lst *l);
void				ft_skip_meanless_word(t_lexer_lst *l);
char				*ft_quote(char *str, t_lexer_lst *l, int closed, char quote);
char				*ft_read_argument(t_lexer_lst *l);

/* token_utils.c */
char				*ft_strjoin(char *s1, char *s2);
int					ft_strlen(const char *str);
char				*ft_strndup(const char *str, int n);
char				*ft_strchr(const char *str, int c);

/* lexer.c */
t_lexer_lst			*ft_init_lexer(char *str);
void				ft_analyze_string(t_lexer_lst *out_l);

#endif
