/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 15:51:33 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/12 03:42:21 by kanghyki         ###   ########.fr       */
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
# define WHITE_SPACE " \r\v\f\t\n"
# define QUOTE "\"'"
# define SKIPCHAR "|;>< \r\v\f\t\n\"'$"

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

typedef struct s_token {
	enum e_token_type		token_type;
	char					*pa_str;
	struct s_token			*next;
} t_token;

typedef struct s_argument {
	enum e_token_type		next_token_type;
	char					**pa_argument;
	char					***env;
	struct s_argument		*next;
} t_argument;

void	print_memory_test(char *dst, int size);

/*
 * #########################################################
 * #                                                       #
 * #                src/parser/tokenizer/                  #
 * #                                                       #
 * #########################################################
 */
/* src/parser/tokenizer/token.c */
t_token			*ft_init_token(char *pa_str, enum e_token_type token_type);
void			ft_add_token_back(t_token **head, t_token *new_token);
void			ft_merge_environment(char **str, char **dst, char **env);
void			ft_quote(char **str, char **dst, char quote, char **env);
t_token			*ft_create_token_type_metachar(char **str);
t_token			*ft_create_token_type_argument(char **input_command, char **environment);
/* Interface */
t_token	*ft_tokenizer(char *input_command, char **environment);
/* tokenizer utils */
char			*ft_strchr_except_null(const char *str, int c);
char			*ft_strndup(const char *src, size_t n);

/* tokenizer, parser utils */
void			ft_merge_string(char **dst, char *src);
/*
 * #########################################################
 * #                                                       #
 * #                       src/parser/                     #
 * #                                                       #
 * #########################################################
 */
/* src/parser/parser.c */
t_argument		*ft_init_argument(void);
char			**ft_init_pa_argument(t_token *cur_token);
void			ft_add_argument_back(t_argument **head, t_argument *argument);
char	*ft_get_token_type_char(enum e_token_type token_type);
t_token	*ft_add_additional_pipe(t_token *cur_token, char **env);
void	ft_heredoc(t_token *cur_token, char **env);
t_token			*ft_read_token(t_token *cur_token, t_argument *argument, int index);
t_token			*ft_read_token_initial_state(t_token *cur_token, t_argument *argument, int index);
/* Interface */
t_argument	*ft_parser(char *input_command, char ***environment);
/* Clean up */
void			ft_free_token_memory(t_token *token);
void			ft_free_argument_memory(t_argument *arg);

/*
 * #########################################################
 * #                                                       #
 * #                          src/                         #
 * #                                                       #
 * #########################################################
 */

/* src/env_utils.c */
int				ft_strdplen(char **str);
char			**ft_copy_env(char **env);
int				ft_is_match_key(char *env, char *key);
void			ft_delete_env(char **env);
char			**ft_find_matched_key(char **env, char *key);
char			**ft_add_dict_to_env(char **env, char *str);
char			**ft_remove_dict_from_env(char **env, char *key);
char			**ft_unset_env(char **env, char *key);
char			**ft_set_env(char **env, char *new_dict);
void			ft_print_env(char **env);
char			*ft_key_to_value(char **env, char *key);

/* src/environment_execute.c */
int				ft_strcmp(const char *s1, const char *s2);
void			ft_execute_export(t_argument *arg);
void			ft_execute_env(t_argument *arg);
void			ft_execute_unset(t_argument *arg);
void			ft_env_simple_command_test(t_argument *arg);

#endif
