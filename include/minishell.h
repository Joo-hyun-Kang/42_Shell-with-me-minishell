/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jokang <jokang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 15:51:33 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/22 17:50:45 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../library/libft/inc/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <termios.h>

# define M_META		"|;><"
# define M_SPACE	" \r\v\f\t"
# define M_QUOTE	"\"'"
# define M_ENV		'$'
# define M_HOME		'~'
# define M_BSLASH	'\\'
# define M_SEP		"|;>< \r\v\f\t\"'$~\\"

# define F_HEREDOC	"doc.here"

# define BANNER "\
                                                    \n\
                                                    \n\
     \033[1;35mS  H  E  L  L\
          \033[1;33mW  I  T  H\
          \033[1;32mM  E\033[0;0m\n\
     \033[1;37m-  -  -  -  -          -  -  -  -          -  -\033[0;0m\n\
                                                    \n\
                                                    \n"

#define READLINE "\
\033[1;35m\
minishell\
\033[0;0m\
-\
\033[1;33m\
0.0\
\033[1;32m\
$\
\033[0;0m\
 "
int	g_exit;

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
}	t_env;

typedef struct s_env_root {
	struct s_env			*root;
}	t_env_root;

typedef struct s_lexer {
	char		*cmd_str;
	int			cmd_str_size;
	int			read_idx;
	char		*spos;
	t_env_root	*env;
	char		*pa_str;
}	t_lexer;

typedef struct s_token {
	enum e_token_type		token_type;
	char					*pa_str;
	struct s_token			*next;
}	t_token;

typedef struct s_argument {
	enum e_token_type		next_token_type;
	char					**pa_argument;
	struct s_env_root		*env;
	struct s_argument		*next;
}	t_argument;

# include "../src/execute_cmd/cmd.h"

/*
 * #########################################################
 * #                                                       #
 * #                       src/signal/                     #
 * #                                                       #
 * #########################################################
 */
/* src/signal/signal.c */
void		ft_set_signal(void);

/*
 * #########################################################
 * #                                                       #
 * #                src/parser/tokenizer/                  #
 * #                                                       #
 * #########################################################
 */
/* src/parser/tokenizer/tokenizer.c */
t_token		*ft_tokenizer(char *cmd_str, t_env_root *env);

/* src/parser/tokenizer/tokenizer_core.c */
t_token		*ft_create_token_type_metachar(t_lexer *lexer);
t_token		*ft_create_token_type_argument(t_lexer *lexer);

/* src/parser/tokenizer/tokenizer_merge.c */
void		ft_merge_env(t_lexer *lexer);
void		ft_merge_quote(t_lexer *lexer, char quote);

/* src/parser/tokenizer/tokenizer_merge_utils.c */
void		ft_merge_home(t_lexer *lexer);
void		ft_merge_bslash(t_lexer *lexer);
void		ft_save_str(t_lexer *lexer);

/* src/parser/tokenizer/tokenizer_common_utils.c */
t_token		*ft_init_token(char *str, enum e_token_type t_type);
void		ft_free_token(t_token *token);
void		ft_add_token_back(t_token **head, t_token *new_token);

/* src/parser/tokenizer/lexer.c */
t_lexer		*ft_init_lexer(char *cmd_str, t_env_root *env);
void		ft_free_lexer(t_lexer *lexer);
void		ft_set_spos(t_lexer *lexer);
char		*ft_get_spos(t_lexer *lexer);
t_env_root	*ft_get_env(t_lexer *lexer);

/* src/parser/tokenizer/lexer_utils.c */
void		ft_read_lexer(t_lexer *lexer);
char		ft_cur_char(t_lexer *lexer);
char		*ft_cur_ptr(t_lexer *lexer);
char		ft_next_char(t_lexer *lexer);
void		ft_replace_lexer_cmd_str(t_lexer *lexer, char *new_cmd_str);

/* src/parser/tokenizer/ft_strchr_except_null.c */
char		*ft_strchr_except_null(const char *str, int c);

/*
 * #########################################################
 * #                                                       #
 * #                      src/parser/                      #
 * #                                                       #
 * #########################################################
 */
/* src/parser/parser.c */
void		ft_syntax_error(enum e_token_type token_type);
t_token		*ft_read_token_end(t_token *cur_token, t_argument *arg);
t_token		*ft_read_token_mid(t_token *cur_token, t_argument *arg, int idx);
t_token		*ft_read_token_init(t_token *cur_token, t_argument *arg, int idx);
t_argument	*ft_parser(char *cmd_str, t_env_root *root_env);

/* src/parser/parser_free_utils.c */
void		ft_free_argument(t_argument *arg);

/* src/parser/parser_common_utils.c */
t_argument	*ft_init_argument(t_token *cur_token, t_env_root *env);
char		**ft_init_pa_argument(t_token *cur_token);
void		ft_add_argument_back(t_argument **head, t_argument *arg);

/* src/parser/parser_add_pipe.c */
t_token		*ft_add_pipe(t_token *cur_token, t_env_root *env);

/* src/parser/parser_heredoc.c */
t_token		*ft_heredoc(t_argument *arg, t_token *cur_token);

/* src/parser/ft_merge_str.c */
char		*ft_merge_str(char *s1, char *s2);

/*
 * #########################################################
 * #                                                       #
 * #                    src/environment/                   #
 * #                                                       #
 * #########################################################
 */ /* src/environment/env_utils.c */
bool		ft_is_dictionary(char *str);
char		*ft_extract_key_from_str(char *str);
char		*ft_extract_value_from_str(char *str);
void		ft_free_env_node(t_env *node);
t_env_root	*ft_dpenv_to_bstenv(char **env);

/* src/environment/environment_bst/env_bst_init.c */
t_env_root	*ft_init_env_root(void);
t_env		*ft_init_env_node(char *key, char *value);

/* src/environment/environment_bst/env_bst_insert.c */
void		ft_env_insert(t_env_root *root, char *key, char *value);

/* src/environment/environment_bst/env_bst_search.c */
t_env		*ft_env_search(t_env_root *root, char *key);

/* src/environment/environment_bst/env_bst_delete.c */
void		ft_env_delete(t_env_root *root, char *key);

#endif
