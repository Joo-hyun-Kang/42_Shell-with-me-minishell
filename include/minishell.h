/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jokang <jokang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 15:51:33 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/24 00:17:16 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../library/libft/inc/libft.h"
# include "../library/get_next_line/inc/get_next_line.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <termios.h>

# define M_META		"|><"
# define M_SPACE	" \r\v\f\t"
# define M_QUOTE	"\"'"
# define M_ENV		'$'
# define M_HOME		'~'
# define M_SEP		"|>< \r\v\f\t\"'$~"

# define F_HEREDOC	"heredoc.tmp"
# define F_EXTRA	"extra.tmp"

# define BANNER "\
                                                    \n\
                                                    \n\
     \033[1;35mS  H  E  L  L          \033[1;33mW  I  T  H          \033[1;32mM  E\033[0;0m\n\
     \033[1;37m-  -  -  -  -          -  -  -  -          -  -\033[0;0m\n\
                                                    \n\
                                                    \n"

# define READLINE "\033[1;35mminishell\033[0;0m-\033[1;33m0.0\033[1;32m$\033[0;0m "
int	g_exit;

enum e_token_type {
	ARGUMENT,
	PIPE,					// |
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
	int						size;
	struct s_env			*root;
}	t_env_root;

typedef struct s_lexer {
	char		*cmd_str;
	int			cmd_str_size;
	int			read_idx;
	char		*pos;
	t_env_root	*env;
	char		*pa_str;
	bool		err;
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
void		ft_sigint(int sig);
void		ft_sigquit(int sig);
void		ft_sig_get_extra_line(int sig);

/*
 * #########################################################
 * #                                                       #
 * #                      src/parser/                      #
 * #                                                       #
 * #########################################################
 */
/* src/parser/parser.c */
t_argument	*ft_parser(char *cmd_str, t_env_root *root_env);

/* src/parser/parser_utils.c */
t_argument	*ft_init_argument(t_token *cur_token, t_env_root *env);
char		**ft_init_pa_argument(t_token *cur_token);
void		ft_add_argument_back(t_argument **head, t_argument *arg);
void		ft_free_argument(t_argument *arg);
t_argument	*p_all_free(t_argument *head_arg, t_token *head_tok);

/* src/parser/ft_merge_str.c */
char		*ft_merge_str(char *s1, char *s2);

/* src/parser/extra_pipe.c */
t_token		*p_extra_pipe(t_token *cur_token, t_env_root *env);

/* src/parser/heredoc.c */
t_token		*p_heredoc(t_argument *arg, t_token *cur_token);

/* src/parser/heredoc_utils.c */
t_token		*p_heredoc_err(int status);
void		p_heredoc_child(t_argument *arg, t_token *cur_token);
char		*p_read_heredoc(char *heredoc);
char		*p_env_heredoc(char *str, t_env_root *env);
void		p_replace_env_heredoc(char **str, char **dst, t_env_root *env);

/*
 * #########################################################
 * #                                                       #
 * #                src/parser/tokenizer/                  #
 * #                                                       #
 * #########################################################
 */
/* src/parser/tokenizer/tokenizer.c */
t_token		*ft_tokenizer(char *cmd_str, t_env_root *env);

/* src/parser/tokenizer/tokenizer_arg.c */
void		tk_replace_env(t_lexer *lexer);
void		tk_replace_home(t_lexer *lexer);
void		tk_quote(t_lexer *lexer);

/* src/parser/tokenizer/tokenizer_utils.c */
t_token		*tk_init(t_lexer *lexer, enum e_token_type t_type);
void		tk_free(t_token *token);
void		tk_add_back(t_token **head, t_token *new_token);

/* src/parser/tokenizer/lexer.c */
t_lexer		*lx_init(char *cmd_str, t_env_root *env);
void		lx_free(t_lexer *lexer);

/* src/parser/tokenizer/lexer_utils.c */
void		lx_read(t_lexer *lexer);
char		lx_chr(t_lexer *lexer);
char		*lx_ptr(t_lexer *lexer);
char		lx_next_chr(t_lexer *lexer);

/* src/parser/tokenizer/lexer_extra_utils.c */
void		lx_set_pos(t_lexer *lexer);
char		*lx_get_pos(t_lexer *lexer);
t_env_root	*lx_env(t_lexer *lexer);
void		lx_store_str(t_lexer *lexer);

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
void	ft_copy_bstenv_recursive(t_env *node, char **env, int *idx);
char	**ft_copy_bstenv(t_env_root *root);
void	ft_remove_copy_env(char **env);
void	ft_print_copy_env(char **env);

/* src/environment/environment_bst/env_bst_init.c */
t_env_root	*ft_init_env_root(void);
t_env		*ft_init_env_node(char *key, char *value);

/* src/environment/environment_bst/env_bst_insert.c */
void		ft_env_insert(t_env_root *root, char *key, char *value);

/* src/environment/environment_bst/env_bst_search.c */
t_env		*ft_env_search(t_env_root *root, char *key);

/* src/environment/environment_bst/env_bst_delete.c */
void		ft_env_delete(t_env_root *root, char *key);

/*
 * #########################################################
 * #                                                       #
 * #                      src/prompt/                      #
 * #                                                       #
 * #########################################################
 */
/* src/prompt/echo.c */
void		ft_set_echo(void);
void		ft_set_noecho(void);
void		ft_exit(void);

#endif
