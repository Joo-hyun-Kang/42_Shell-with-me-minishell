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

/* src/execute_cmd/pipe.c */

void		ft_execute_pipe(t_argument **arg, int state, t_pipes *pipes);
int			ft_construt_pipes(t_argument *arg, t_pipes *pipes);
void		ft_set_pipe(t_pipes *pipes, int state);

/* src/execute_cmd/redir.c */

int			ft_is_redir(enum e_token_type token);
void		ft_execute_redir(t_argument **arg, int state, t_pipes *pipes);
void		ft_sort_redir_command(t_argument **arg, t_arraylist *list_arg, t_arraylist *list_com);
int			ft_set_redir(t_pipes *pipes, t_arraylist *list_arg, t_arraylist *list_com, t_arraylist *argument);
void		ft_relocate_redir_argument(t_argument **arg);
int			ft_find_next_pipe(t_argument **arg);

/* src/execute_cmd/arraylist.c */

int			add_arraylist(t_arraylist *arraylist, char* value, int type);
int			is_arraylist_full(t_arraylist *arraylist);
int			allocate_arraylist(t_arraylist *arraylist);
void		ft_free_list_value(char **pa_char, int lenght);
void	    ft_copy_char_arr_malloc(char **dst, char **src);
void		exit_malloc_fail(void *p);
void		free_arraylist(t_arraylist *arraylist);

#endif

// 1-1. < 여러 개 인 경우
// <가 중복되면 가장 뒤에 있는 걸 처리
// cat < 4 1 2 3 == cat 1 2 3 < 4 == cat 1 2 3

// 1-2. < 뒤에 매개변수가 많아지는 경우
// < 뒤에 매개변수가 많아지면 < 효력없음

// 2-1. > 여러 개 인경우
// 2-2처럼 가장 첫번째 나오는 파일을 만듬
// 2-2처럼 매개변수가 많지 않는 한 마지막 걸 그냥 출력
// 2-2처럼 >에 여러 개가 있는 경우 중복 처리

// 2-2. > 뒤에 매개변수가 많아지는 경우
// > 뒤에 매개변수가 많아지면 요거 파일을 만들지만(오픈은 함)
// 뒤에 있는 매개변수를 커맨드가 처리함
// c2r2s7:tmp jokang$ ls > a b c
// ls: b: No such file or directory
// ls: c: No such file or directory
// c2r2s7:tmp jokang$ cat a --> 아무것도 출력이 안 된다

// 3-1. >> 여러 개 인경우
// 3-3처럼 가장 첫번째 나오는 파일을 만듬
// 3-3처럼 매개변수가 많지 않는 한 마지막 걸 그냥 출력
// 3-3처럼 >>에 여러 개가 있는 경우 중복 처리

// 3-2. >> 뒤에 매개변수가 많아지는 경우
// >> 뒤에 매개변수가 많아지면 요거 파일을 만들어서 뒤에 덮붙이거나 새로 만듬
// 뒤에 있는 매개변수를 커맨드가 처리함

// 혼합
// c2r2s7:tmp jokang$ ls > a 1 2 < 3 4
// c2r2s7:tmp jokang$ cat a
// 1
// 2
// 4
// c2r2s7:tmp jokang$


// ls > a.txt
// 	{ next tokenType: LT }
// { arg->pa_argument: ls }
// { next tokenType: EOL }
// { arg->pa_argument: a.txt }

//minishell-4.2$ < a.txt cat
// { next tokenType: GT }
// { next tokenType: EOL }
// { arg->pa_argument: a.txt }
// { arg->pa_argument: cat }

// minishell-4.2$ cat < a.txt < b.txt < c.txt
// { next tokenType: GT }
// { arg->pa_argument: cat }
// { next tokenType: GT }
// { arg->pa_argument: a.txt }
// { next tokenType: GT }
// { arg->pa_argument: b.txt }
// { next tokenType: EOL }
// { arg->pa_argument: c.txt }