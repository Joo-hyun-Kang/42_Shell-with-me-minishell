/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 17:18:03 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/03 02:56:29 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H 
# define TOKEN_H 

# include <stdlib.h>
# include <stdbool.h>

enum e_token_type {
	FUNCTION,
	ARGUMENT,
	MINUS_SIGN,				// -
	PIPE,					// |
	SEMICOLON,				// ;
	SINGLE_QUOTE,			// '
	DOUBLE_QUOTE,			// "
	DOLLER_SIGN,			// $
	SLASH,					// /
	/* Redirections */
	LT,						// > (little)
	DLT,					// >>
	GT,						// < (greater)
	DGT,					// <<
	QUESTION_MAKR			// ?
};

typedef struct s_token_lst {
	char					*token;
	enum e_token_type		t_type;			// Token type
	struct s_token_lst		*next;
	struct s_token_lst		*prev;
} t_token_lst;

typedef struct s_cmd_lst {
	char					*cmd;
	int						r_pos;			// Current read position.
	char					ch;				// Current read position character.
	struct s_token_lst		*head_token;
	struct s_cmd_lst		*next;
	struct s_cmd_lst		*prev;
} t_cmd_lst;

/* token.c */
t_cmd_lst			*ft_init_cmd(char	*str);
t_token_lst			*ft_new_token(enum e_token_type type, char *str);
t_token_lst 		*ft_next_token(t_cmd_lst *cl);
void				ft_read_char(t_cmd_lst *cl);
char				ft_peek_char(t_cmd_lst *cl);
void				ft_skip_whitespace(t_cmd_lst *cl);

/* token_utils.c */
bool				ft_is_letter(char c);
bool				ft_is_digit(char c);
int					ft_strlen(char *str);
char				*ft_strndup(const char *str, size_t n);
char				*ft_strchr(const char *str, int c);

/* commnad.c */
t_cmd_lst			*ft_init_cmd(char *str);
void				ft_set_cmd_lst(t_cmd_lst *out_cl);
t_cmd_lst			*ft_get_cmd_lst(char *str);

#endif
