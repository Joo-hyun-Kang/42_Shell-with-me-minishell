#include "../../include/minishell.h"

int	is_builtin(char *command, enum e_btype *out_type)
{
	if (ft_strcmp(command, "echo") == 0)
		*out_type = BUL_ECHO;
	else if (ft_strcmp(command, "cd") == 0)
		*out_type = BUL_CD;
	else if (ft_strcmp(command, "pwd") == 0)
		*out_type = BUL_PWD;
	else if (ft_strcmp(command, "export") == 0)
		*out_type = BUL_EXPORT;
	else if (ft_strcmp(command, "unset") == 0)
		*out_type = BUL_UNSET;
	else if (ft_strcmp(command, "env") == 0)
		*out_type = BUL_ENV;
	else if (ft_strcmp(command, "exit") == 0)
		*out_type = BUL_EXIT;
	else
		*out_type = INVAILD;
	if (*out_type == INVAILD)
		return (false);
	return (true);
}

void	ft_builtin(t_argument *arg, enum e_btype bull_type, int is_parent)
{
	if (bull_type == BUL_ECHO)
		ft_execute_echo(arg, is_parent);
	else if (bull_type == BUL_CD)
		ft_execute_cd(arg, is_parent);
	else if (bull_type == BUL_PWD)
		ft_execute_pwd(is_parent);
	else if (bull_type == BUL_EXIT)
		ft_execute_exit(arg, is_parent);
	else if (bull_type == BUL_EXPORT)
		ft_execute_export(arg, is_parent);
	else if (bull_type == BUL_ENV)
		ft_execute_env(arg, is_parent);
	else if (bull_type == BUL_UNSET)
		ft_execute_unset(arg, is_parent);
}
