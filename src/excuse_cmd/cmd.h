#include "../../include/minishell.h"

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

#define PIPE_READ (0)
#define PIPE_WRITE (1)

#define NULL_POSITOIN (1)
#define BIN_SH_POSIOTON (1)
#define BIN_SH_ARG_POSITION (1)

enum e_bulltein_type {
    BUL_ECHO,
    BUL_CD,
    BUL_PWD,
    BUL_EXPORT,
    BUL_UNSET,
    BUL_ENV,
    BUL_EXIT,
	INVAILD
};

void	ft_system(t_argument *argument);


void	ft_execute_echo(t_argument *argument);
void	ft_execute_pwd(t_argument *argument);
void	ft_execute_cd(t_argument *argument);

int		is_bulletin(char *command, enum e_bulltein_type *out_type);

void	ft_print_error();
void	ft_free_argument(t_argument *pa_argument);
int     ft_get_length_2d_arr(char **array);
void	ft_get_sh_command(char **src_2d, char **out_dst);
void	ft_free_command(char **pa_char);
