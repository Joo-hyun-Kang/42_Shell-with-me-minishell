#ifndef CMD_H
# define CMD_H

# include "../../include/minishell.h"
# include <unistd.h>
# include <sys/wait.h>
# include <dirent.h>
# include <errno.h>
# include <string.h>
# include <fcntl.h>

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

#define ENV_PATH_NAME_LENGTH (5)

// out_copy는 마지막 파이프의 READ fd값을 가짐
// 짝수일 때는 pipe1의 READ를 홀수일 때는 pipe2의 READ를 가짐
typedef struct pipes {
	int			**array;
	size_t		pipe_count;
	size_t		current_idx;
} t_pipes;

enum redir_type {
	RE_COM,
	LT_OPEN,
	DLT_OPEN,
	GT_FILE,
	NONE
};

typedef struct arraylist {
	char	**pa_arr;
    int     *type;
	int		length;
	int		capacity;
}	t_arraylist;

typedef struct redir_var {
	t_arraylist	*list_arg;
	t_arraylist *list_com;
	t_pipes		*pipes;
	int			will_stdin_pipe;
	int			will_stdout_pipe;
}	t_redir_var;

/*
 * #########################################################
 * #                                                       #
 * #                    src/execute_cmd/                   #
 * #                                                       #
 * #########################################################
 */
/* src/execute_cmd/cmd.c */
void				ft_print_error();
void				ft_execute_single_cmd(t_argument **arg);
void				ft_system(t_argument *argument);
int					ft_execute_path(t_argument *arg);
int					ft_execute_nopath(t_argument *arg, char *pa_path);
int					ft_execute_except_case(t_argument *arg);
void				ft_execute(t_argument *argument, int is_parent);
int					ft_is_command_dir();
char	            *ft_search_command_path_malloc(t_env_root *root, char *command);
char				*ft_join_path_command_malloc(char *path, char *command);
void				ft_free_command(char **pa_char);
int					ft_get_length_2d_arr(char **array);
void            	ft_execute_mult_cmd(t_argument **arg);
void				ft_wait_child(pid_t child_pid);

/* src/execute_cmd/ft_atoull.c */
unsigned long long	ft_atoull(const char *str, int *is_numeric);

/* src/environment/execute_env.c */
void				ft_execute_export(t_argument *arg, int is_parent);
void				ft_execute_env(t_argument *arg, int is_parent);
void				ft_execute_unset(t_argument *arg, int is_parent);

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

int			ft_is_redir(enum e_token_type token);
pid_t		ft_execute_redir(t_argument **arg, int state, t_pipes *pipes);
void		ft_sort_redir_command(t_argument **arg, t_arraylist *list_arg, t_arraylist *list_com);
int			ft_set_redir(t_redir_var *redir, t_arraylist *argument);
void		ft_relocate_redir_argument(t_argument **arg);
int			ft_find_next_pipe(t_argument **arg);

/* src/execute_cmd/arraylist.c */

void		init_arraylist(t_arraylist *arraylist);
int			add_arraylist(t_arraylist *arraylist, char* value, int type);
int			is_arraylist_full(t_arraylist *arraylist);
int			allocate_arraylist(t_arraylist *arraylist);
void		ft_free_list_value(char **pa_char, int lenght);
void	    ft_copy_char_arr_malloc(char **dst, char **src);
void		exit_malloc_fail(void *p);
void		free_arraylist(t_arraylist *arraylist);

enum e_err_code {
	CD_MIN,
	CD_NO_DIR,
	CD_NO_SET,
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
	EXE_IS_DIR,
	EXE_NO_DIR,
	EXE_MAX
};

void	ft_error(enum e_err_code error_code, void *info, int is_parent);

#endif