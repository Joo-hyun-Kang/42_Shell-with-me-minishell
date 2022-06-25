#include "cmd.h"

void	ft_fork_execute(t_argument *argument)
{
	pid_t	pid;

	pid = fork_safe();
	if (pid == 0)
		ft_execute(argument, false);
	ft_wait_child(pid);
}

void	ft_execute_single_cmd(t_argument **arg)
{
	enum e_btype		bull_type;

	if (is_builtin((*arg)->pa_argument[COMMAND_POSITION], &bull_type) == true)
		ft_builtin((*arg), bull_type, true);
	else
		ft_fork_execute(*arg);
	(*arg) = (*arg)->next;
}

void	ft_wait_child(pid_t child_pid)
{
	pid_t	pid;
	int		status;

	status = 0;
	while (true)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == child_pid)
			g_exit = (status >> 8);
		else if (pid == -1)
			break ;
	}
}

void	ft_free_command(char **pa_char)
{
	int	i;

	i = 0;
	while (pa_char[i] != NULL)
	{
		free(pa_char[i]);
		i++;
	}
	free(pa_char);
}

int	ft_get_length_2d_arr(char **array)
{
	int	i;

	i = 0;
	while (array[i] != NULL)
	{
		i++;
	}
	return (i);
}
