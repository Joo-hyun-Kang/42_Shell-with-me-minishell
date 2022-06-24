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

void		ft_get_redir_state(t_argument **arg, t_redir *redir, int state);
void		ft_init_redir_variable(t_redir *redir);
t_argument	ft_get_sorted_command(t_lst *redir_arg, t_argument **arg);
pid_t		ft_execute_redir(t_argument **arg, int state, t_pipes *pipes);
pid_t		ft_redir_recursive(t_argument **arg, int state, t_pipes *pipes, int pid);

/* src/execute_cmd/redir_utils.c */

int			ft_is_redir(enum e_token_type token);
int			ft_find_next_pipe(t_argument **arg);
void		ft_free_redir_variable(t_redir *redir);

/* src/execute_cmd/redir_sort.c */
void   		ft_copy_origin_command(t_argument **arg, t_lst *list_com);
void   		ft_get_gt_variable(t_argument *p, t_lst *list_arg, char **gt_file);
void		ft_get_lt_dlt(t_argument *p, t_lst *l_arg, t_lst *l_com, int token);
void		ft_sort_redir_command(t_argument **arg, t_lst *list_arg, t_lst *list_com);

/* src/execute_cmd/redir_set.c */
int			ft_set_redir(t_redir *redir, t_lst *argument);
void		ft_split_command(t_redir *redir, t_lst *arg, t_lst **files, char **gt);
void		ft_set_opne_file(t_lst *open_files, t_redir *redir);

/* src/execute_cmd/arraylist.c */
void		init_arraylist(t_lst *arraylist);
int			add_arraylist(t_lst *arraylist, char* value, int type);
int			allocate_arraylist(t_lst *arraylist);
void		allocate_list_first(t_lst *arraylist);
void		allocate_list_sec(t_lst *arraylist);

/* src/execute_cmd/arraylist_utils.c */
void		ft_free_list_value(char **pa_char, int lenght);
void	    ft_copy_char_arr_malloc(char **dst, char **src);
void		exit_malloc_fail(void *p);
void		free_arraylist(t_lst *arraylist);
int			is_arraylist_full(t_lst *arraylist);

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

void	ft_error(enum e_err_code error_code, void *info, int is_parent);

#endif
