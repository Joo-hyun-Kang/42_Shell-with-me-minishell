/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jokang <jokang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 15:51:33 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/25 19:41:19 by jokang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <termios.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdbool.h>
# include <sys/wait.h>
# include <dirent.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../library/libft/inc/libft.h"
# include "../library/get_next_line/inc/get_next_line.h"

# define M_META		"|><"
# define M_SPACE	" \r\v\f\t"
# define M_QUOTE	"\"'"
# define M_ENV		'$'
# define M_HOME		'~'
# define M_SEP		"|>< \r\v\f\t\"'$~"

# define F_HEREDOC	".heredoc.tmp"
# define F_EXTRA	".extra.tmp"

# define BANNER "\
                                                    \n\
                                                    \n\
     \033[1;35mS  H  E  L  L          \033[1;33mW  I  T  H          \033[1;32mM  E\033[0;0m\n\
     \033[1;37m-  -  -  -  -          -  -  -  -          -  -\033[0;0m\n\
                                                    \n\
                                                    \n"

# define STT_READ "\033[1;34m❯ \033[0;0m\033[1;35mminishell\033[0;0m"
# define END_READ "\033[1;32m$\033[0;0m "

/* PIPE */
# define PIPE_READ (0)
# define PIPE_WRITE (1)

# define NULL_POSITOIN (1)
# define BIN_SH_POSIOTON (1)
# define BIN_SH_ARG_POSITION (1)

# define COMMAND_POSITION (0)
# define COMMAND_ARG_POSITION (1)

# define PIPE_COUNT (2)
# define PIPE1_READ (0)
# define PIPE2_READ (1)

/* STATE */
# define INIT (0)
# define END (1)
# define PIPE_START (2)
# define PIPE_MIDDLE (3)
# define PIPE_END (4)
# define REDIRECTION (5)

# define ENV_PATH_NAME_LENGTH (5)

int	g_exit;

enum e_token_type {
	ARGUMENT,
	PIPE,
	LT,
	DLT,
	GT,
	DGT,
	EOL
};

enum e_btype {
	BUL_ECHO,
	BUL_CD,
	BUL_PWD,
	BUL_EXPORT,
	BUL_UNSET,
	BUL_ENV,
	BUL_EXIT,
	INVAILD
};

enum e_redir_type {
	RE_COM,
	LT_OPEN,
	DLT_OPEN,
	GT_FILE,
	NONE
};

enum e_err_code {
	CD_MIN,
	CD_NO_DIR,
	CD_NO_SET,
	CD_NOT_DIR,
	CD_MAX,
	PWD_MIN,
	PWD_ERR,
	PWD_MAX,
	EXIT_MIN,
	EXIT_NUMERIC,
	EXIT_MANY_ARG,
	EXIT_MAX,
	UNSET_MIN,
	UNSET_INVALID,
	UNSET_MAX,
	EXPORT_MIN,
	EXPORT_INVALID,
	EXPORT_MAX,
	EXE_MIN,
	EXE_CMD_NOT,
	EXE_NO_DIR,
	EXE_PERMISSION,
	EXE_MAX
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

// out_copy는 마지막 파이프의 READ fd값을 가짐
// 짝수일 때는 pipe1의 READ를 홀수일 때는 pipe2의 READ를 가짐
typedef struct pipes {
	int		**array;
	int		pipe_count;
	int		current_idx;
}	t_pipes;

typedef struct arraylist {
	char	**pa_arr;
	int		*type;
	int		length;
	int		capacity;
}	t_lst;

typedef struct redir_var {
	t_lst		*list_arg;
	t_lst		*list_com;
	t_lst		*redir_arg;
	t_pipes		*pipes;
	int			will_stdin_pipe;
	int			will_stdout_pipe;
}	t_redir;

/*
 * #########################################################
 * #                                                       #
 * #                       src/malloc                      #
 * #                                                       #
 * #########################################################
 */
/* src/malloc/ft_strdup.c */
char		*ft_strdup(const char *s);

/* src/malloc/ft_strndup.c */
char		*ft_strndup(const char *src, size_t n);

/*
 * #########################################################
 * #                                                       #
 * #                       src/safe                        #
 * #                                                       #
 * #########################################################
 */
/* src/safe/malloc_safe.c */
void		*malloc_safe(size_t n);

/* src/safe/fork_safe.c */
pid_t		fork_safe(void);

/* src/safe/pipe_safe.c */
int			pipe_safe(int pp[2]);

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
void		ft_sig_for_child(int sig);

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
void		p_heredoc_child(t_argument *arg, t_token *cur_token);
char		*p_env_heredoc(char *str, t_env_root *env);
void		p_replace_env_heredoc(char **str, char **dst, t_env_root *env);

/* src/parser/align_arg.c */
t_argument	*p_align_arg(t_argument *arg);

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

/* src/environment/environment_bst/env_cpy.c */
t_env_root	*ft_dpenv_to_bstenv(char **env);
char		**ft_bstenv_to_dpenv(t_env_root *root);
void		ft_remove_copy_env(char **env);

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
/* src/prompt/ft_echo.c */
void		ft_set_echo(void);
void		ft_set_noecho(void);

/* src/prompt/ft_exit.c */
void		ft_exit(void);

/* src/prompt/ft_readline.c */
char		*ft_readline(void);

/*
 * #########################################################
 * #                                                       #
 * #                     src/builtin/                      #
 * #                                                       #
 * #########################################################
 */
/* src/builtin/builtin.c */
int			is_builtin(char *command, enum e_btype *out_type);
void		ft_builtin(t_argument *arg, enum e_btype bull_type, int is_parent);

/* src/builtin/export.c */
void		ft_execute_export(t_argument *arg, int is_parent);

/* src/builtin/env.c */
void		ft_execute_env(t_argument *arg, int is_parent);

/* src/builtin/unset.c */
void		ft_execute_unset(t_argument *arg, int is_parent);

/* src/builtin/exit.c */
void		ft_execute_exit(t_argument *arg, int is_parent);

/* src/builtin/echo.c */
void		ft_execute_echo(t_argument *arg, int is_parent);

/* src/builtin/pwd.c */
void		ft_execute_pwd(int is_parent);

/* src/builtin/cd.c */
void		ft_execute_cd(t_argument *arg, int is_parent);

/*
 * #########################################################
 * #                                                       #
 * #                    src/execute_cmd/                   #
 * #                                                       #
 * #########################################################
 */
/* src/execute_cmd/cmd.c */
void		ft_system(t_argument *argument);
void		ft_execute_mult_cmd(t_argument **arg);
char		*ft_join_path_command(char *path, char *command);

/* src/execute_cmd/cmd_single.c */
void		ft_fork_execute(t_argument *argument);
void		ft_execute_single_cmd(t_argument **arg);
void		ft_wait_child(pid_t child_pid);
void		ft_free_command(char **pa_char);
int			ft_get_length_2d_arr(char **array);

/* src/execute_cmd/execute.c */
void		ft_execute(t_argument *argument, int is_parent);
int			ft_execute_path(t_argument *arg);
void		ft_execute_error(char *command, int is_parent);

/* src/execute_cmd/execute_path.c */
int			ft_execute_nopath(t_argument *arg, char *pa_path);
char		*ft_search_command_path(t_env_root *root, char *command);
void		ft_find_dir_pos(char *command, char **pa_directories, int *p);

/* src/execute_cmd/execute_except.c */
void		ft_execute_except_case(t_argument *arg);
int			ft_is_command_dir(void);
int			ft_is_command_exist(char *current_path, t_argument *arg);
void		ft_exe_except(char *pa_path, t_argument *arg, char **env);

/* src/environment/execute_env.c */
void		ft_execute_export(t_argument *arg, int is_parent);
void		ft_execute_env(t_argument *arg, int is_parent);
void		ft_execute_unset(t_argument *arg, int is_parent);

/* src/execute_cmd/pipe.c */

void		ft_get_pipe_state(int *state);
pid_t		ft_execute_pipe(t_argument **arg, int state, t_pipes *pipes);
int			ft_construt_pipes(t_argument *arg, t_pipes *pipes);
void		ft_set_pipe(t_pipes *pipes, int state);

/* src/execute_cmd/pipe_utils.c */

void		ft_close_pipe(t_pipes *pipes);
void		ft_free_pipes(t_pipes **pipes);
int			ft_get_pipe_count(t_argument *arg);

/* src/execute_cmd/redir.c */

void		ft_get_redir_state(t_argument **arg, t_redir *redir, int state);
void		ft_init_redir_variable(t_redir *redir);
t_argument	ft_get_sorted_command(t_lst *redir_arg, t_argument **arg);
pid_t		ft_execute_redir(t_argument **arg, int state, t_pipes *pipes);
pid_t		ft_redir_re(t_argument **arg, int state, t_pipes *pipes, int pid);

/* src/execute_cmd/redir_utils.c */

int			ft_is_redir(enum e_token_type token);
int			ft_find_next_pipe(t_argument **arg);
void		ft_free_redir_variable(t_redir *redir);
void		ft_make_redir_com(t_redir *redir, t_lst *arg);

/* src/execute_cmd/redir_sort.c */
void		ft_copy_origin_command(t_argument **arg, t_lst *list_com);
void		ft_get_gt_variable(t_argument *p, t_lst *list_arg, char **gt_file);
void		ft_get_lt_dlt(t_argument *p, t_lst *l_arg, t_lst *l_com, int token);
void		ft_sort_redir_command(t_argument **arg, t_lst *l_arg, t_lst *l_com);

/* src/execute_cmd/redir_set.c */
int			ft_set_redir(t_redir *redir, t_lst *argument);
void		split_com(t_redir *redir, t_lst *arg, t_lst **files, char **gt);
void		ft_set_opne_file(t_lst *open_files, t_redir *redir);
void		ft_open_non_write(int j, t_lst *files);
void		ft_open_write(int j, t_lst *files, t_redir *redir);

/* src/execute_cmd/redir_set_utils.c */
void		ft_set_read_file(char *pa_gt_files, t_redir *redir);
void		ft_pipe_redir(t_redir *redir);

/* src/execute_cmd/arraylist.c */
void		init_arraylist(t_lst *arraylist);
int			add_arraylist(t_lst *arraylist, char *value, int type);
int			allocate_arraylist(t_lst *arraylist);
void		allocate_list_first(t_lst *arraylist);
void		allocate_list_sec(t_lst *arraylist);

/* src/execute_cmd/arraylist_utils.c */
void		ft_free_list_value(char **pa_char, int lenght);
void		ft_copy_char_arr_malloc(char **dst, char **src);
void		exit_malloc_fail(void *p);
void		free_arraylist(t_lst *arraylist);
int			is_arraylist_full(t_lst *arraylist);

/* src/execute_cmd/ft_error.c */
int			ft_pwd_error(int error_code, void *free_info);
int			ft_unset_error(int error_code, void *info);
int			ft_export_error(int error_code, void *info);
int			ft_exe_error(int error_code, void *info);
void		ft_error(enum e_err_code error_code, void *info, int is_parent);

/* src/execute_cmd/ft_error_sec.c */
int			ft_cd_error(int error_code, void *free_info);
int			ft_exit_error(int error_code, void *info);

#endif
