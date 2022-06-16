/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 15:51:33 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/16 16:16:39 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../library/libft/inc/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

# define M_META		"|;><"
# define M_SPACE	" \r\v\f\t"
# define M_QUOTE	"\"'"
# define M_ENV		'$'
# define M_HOME		'~'
# define M_SEP		"|;>< \r\v\f\t\"'$~"

# define BANNER "\
                                                    \n\
                                                    \n\
     S  H  E  L  L          W  I  T  H          M  E\n\
     -  -  -  -  -          -  -  -  -          -  -\n\
                                                    \n\
                                   Jokang   Kanghyki\n\
                                   ------ & --------\n\
                                                v0.0\n"

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

typedef struct s_env {
	char					*pa_key;
	char					*pa_value;
	struct s_env			*parent;
	struct s_env			*left;
	struct s_env			*right;
} t_env;

typedef struct s_env_root {
	struct s_env			*root;
} t_env_root;

typedef struct s_lexer {
	char		*cmd_str;
	int			cmd_str_size;
	int			read_idx;
} t_lexer;

typedef struct s_token {
	enum e_token_type		token_type;
	char					*pa_str;
	struct s_token			*next;
} t_token;

typedef struct s_argument {
	enum e_token_type		next_token_type;
	char					**pa_argument;
	struct s_env_root		*env;
	struct s_argument		*next;
} t_argument;

void	print_memory_test(char *dst, int size);

/*
 * #########################################################
 * #                                                       #
 * #                       src/signal/                     #
 * #                                                       #
 * #########################################################
 */
/* src/signal/signal.c */
void			sigint_handler(int sig);
void			sigint_handler_after_parsing(int sig);

/*
 * #########################################################
 * #                                                       #
 * #                src/parser/tokenizer/                  #
 * #                                                       #
 * #########################################################
 */
/* src/parser/tokenizer/tokenizer.c */
t_token			*ft_tokenizer(char *cmd_str, t_env_root *env);

/* src/parser/tokenizer/tokenizer_core.c */
t_token			*ft_create_token_type_metachar(t_lexer *lexer);
t_token			*ft_create_token_type_argument(t_lexer *lexer, t_env_root *env);

/* src/parser/tokenizer/tokenizer_merge.c */
void			ft_merge_env(t_lexer *lexer, char **dst, t_env_root *env);
void			ft_merge_home(t_lexer *lexer, char **dst, t_env_root *env);
void			ft_merge_quote(t_lexer *lexer, char **dst, t_env_root *env, char quote);
void			ft_no_quote(t_lexer *lexer, char **dst, t_env_root *env, char quote);

/* src/parser/tokenizer/tokenizer_utils.c */
char			*ft_strchr_except_null(const char *str, int c);
t_token			*ft_init_token(char *str, enum e_token_type token_type);
void			ft_add_token_back(t_token **head, t_token *new_token);

/* src/parser/tokenizer/lexer.c */
t_lexer			*ft_init_lexer(char *cmd_str);
void			ft_free_lexer(t_lexer *lexer);

/* src/parser/tokenizer/lexer_utils.c */
char			ft_read_lexer(t_lexer *lexer);
char			ft_cur_char(t_lexer *lexer);
char			*ft_cur_ptr(t_lexer *lexer);
char			ft_next_char(t_lexer *lexer);
void			ft_replace_lexer_cmd_str(t_lexer *lexer, char *new_cmd_str);

/*
 * #########################################################
 * #                                                       #
 * #                      src/parser/                      #
 * #                                                       #
 * #########################################################
 */
/* src/parser/parser.c */
char			*ft_get_token_type_char(enum e_token_type token_type);
t_token			*ft_read_token(t_token *cur_token, t_argument *arg, int idx);
t_token			*ft_read_token_init(t_token *cur_token, t_argument *arg, int idx);
t_argument		*ft_parser(char *cmd_str, t_env_root *root_env); /* API */

/* src/parser/parser_free_utils.c */
void			ft_free_token(t_token *token);
void			ft_free_argument(t_argument *arg);

/* src/parser/parser_utils.c */
t_argument		*ft_init_argument(t_token *cur_token, t_env_root *env);
char			**ft_init_pa_argument(t_token *cur_token);
void			ft_add_argument_back(t_argument **head, t_argument *arg);

/* src/parser/parser_additional.c */
t_token			*ft_read_additional_pipe(t_token *cur_token, t_env_root *env);
void			ft_read_additional_heredoc(t_token *cur_token, t_env_root *env);

/* src/parser/ft_merge_str.c */
char			*ft_merge_str(char *s1, char *s2);

/*
 * #########################################################
 * #                                                       #
 * #                    src/environment/                   #
 * #                                                       #
 * #########################################################
 */

/* src/environment/env_execute.c */
void			ft_execute_export(t_argument *arg);
void			ft_execute_env(t_argument *arg);
void			ft_execute_unset(t_argument *arg);
void			ft_env_simple_command_test(t_argument *arg);

/* src/environment/env_tree.c */
t_env_root		*ft_init_env_root(void);
t_env			*ft_init_env_node(char *key, char *value);
void			ft_env_replace_value(t_env *node, char *new_value);
int				ft_env_insert_recursive(char *key, char *value, t_env *cur_node);
void			ft_env_insert(t_env_root *root, char *key, char *value);
t_env			*ft_env_search_recursive(t_env *cur_node, char *key);
t_env			*ft_env_search(t_env_root *root, char *key);
void			ft_env_delete(t_env_root *root, char *key);
int				ft_is_dictionary(char *str);
char			*ft_extract_value_from_node(t_env *node);
char			*ft_extract_key_from_str(char *str);
char			*ft_extract_value_from_str(char *str);
void			ft_free_env_node(t_env *node);
t_env_root		*ft_dpenv_to_bstenv(char **env);

#endif
