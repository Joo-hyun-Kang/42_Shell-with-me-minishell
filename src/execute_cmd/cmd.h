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
	int	pipe1[PIPE_COUNT];
	int	pipe2[PIPE_COUNT];
    int out_copy;
    int out_copy_state;
} t_pipes;

enum e_builtin_type {
    BUL_ECHO,
    BUL_CD,
    BUL_PWD,
    BUL_EXPORT,
    BUL_UNSET,
    BUL_ENV,
    BUL_EXIT,
	INVAILD
};

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
void            	ft_set_pipe(t_pipes *pipes, int state);
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
int         		ft_is_redir(enum e_token_type token);
void            	ft_execute_pipe(t_argument **arg, int state, t_pipes *pipes);
int	                ft_construt_pipes(t_pipes *pipes);

/* src/execute_cmd/builtin.c */
int					is_builtin(char *command, enum e_builtin_type *out_type);
void   				ft_execute_exit(t_argument *argument, int is_parent);
int   				ft_try_cd_parent(t_argument *argumnet);
void				ft_builtin(t_argument *argument, enum e_builtin_type bull_type, int is_parent);
void				ft_execute_echo(t_argument *argument, int is_parent);
void				ft_execute_pwd(t_argument *argument, int is_parent);
void				ft_execute_cd(t_argument *argument, int is_parent);

/* src/execute_cmd/ft_atoull.c */
unsigned long long	ft_atoull(const char *str, int *is_numeric);

/* src/environment/execute_env.c */
void				ft_execute_export(t_argument *arg, int is_parent);
void				ft_execute_env(t_argument *arg, int is_parent);
void				ft_execute_unset(t_argument *arg, int is_parent);

#endif
