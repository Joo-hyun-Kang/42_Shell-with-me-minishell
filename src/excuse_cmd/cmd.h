#include "../../include/minishell.h"

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

#define PIPE_IN (0)
#define PIPE_OUT (1)

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
int		is_bulletin(char *command, enum e_bulltein_type out_type);
void	ft_execute_echo(t_argument *argument);
void	ft_print_error();
void	ft_free_argument(t_argument *pa_argument);