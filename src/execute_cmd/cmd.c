#include "cmd.h"

void	ft_system(t_argument *argument)
{
	if (argument->pa_argument[COMMAND_POSITION] == NULL)
		return ;
	while (argument != NULL)
	{
		if (argument->next_token_type == EOL)
			ft_execute_single_cmd(&argument);
		else
			ft_execute_mult_cmd(&argument);
	}
}

void	ft_execute_mult_cmd(t_argument **arg)
{
	t_pipes				*pa_pipes;
	enum e_token_type	token;
	int					state;
	pid_t				child_pid;

	pa_pipes = (t_pipes *)malloc_safe(sizeof(t_pipes));
	ft_construt_pipes(*arg, pa_pipes);
	state = INIT;
	while (*arg != NULL)
	{
		token = (*arg)->next_token_type;		
		if (token == PIPE)
			child_pid = ft_execute_pipe(arg, state, pa_pipes);
		else if (ft_is_redir(token))
			child_pid = ft_execute_redir(arg, state, pa_pipes);
	}
	ft_close_pipe(pa_pipes);
	ft_wait_child(child_pid);
	ft_free_pipes(&pa_pipes);
}

char	*ft_join_path_command_malloc(char *path, char *command)
{
	char	*pa_temp;
	char	*pa_path_with_command;
	
	pa_temp = ft_strjoin(path, "/");
	pa_path_with_command = ft_strjoin(pa_temp, command);
	free(pa_temp);
	return (pa_path_with_command);
}