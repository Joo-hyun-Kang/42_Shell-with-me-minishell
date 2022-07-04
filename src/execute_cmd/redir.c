/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 11:38:39 by jokang            #+#    #+#             */
/*   Updated: 2022/07/04 20:01:58 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_get_redir_state(t_argument **arg, t_redir *redir, int state)
{
	redir->will_stdin_pipe = false;
	redir->will_stdout_pipe = false;
	if (state == INIT)
	{
		redir->will_stdin_pipe = false;
		if (ft_find_next_pipe(arg) == true)
			redir->will_stdout_pipe = true;
		else
			redir->will_stdin_pipe = false;
	}
	else if (state == PIPE_START || state == PIPE_MIDDLE)
	{
		redir->will_stdin_pipe = true;
		if (ft_find_next_pipe(arg) == true)
			redir->will_stdout_pipe = true;
		else
			redir->will_stdout_pipe = false;
	}
}

void	ft_init_redir_variable(t_redir *redir)
{
	redir->list_arg = (t_lst *)malloc_safe(sizeof(t_lst));
	redir->list_com = (t_lst *)malloc_safe(sizeof(t_lst));
	redir->redir_arg = (t_lst *)malloc_safe(sizeof(t_lst));
	init_arraylist(redir->list_arg);
	init_arraylist(redir->list_com);
	init_arraylist(redir->redir_arg);
}

t_argument	ft_get_sorted_command(t_lst *redir_arg, t_argument **arg)
{
	t_argument	temp;

	temp.next_token_type = EOL;
	temp.pa_argument = redir_arg->pa_arr;
	temp.env = (*arg)->env;
	temp.next = NULL;
	return (temp);
}

pid_t	ft_execute_redir(t_argument **arg, int state, t_pipes *pipes)
{
	t_redir		redir;
	pid_t		pid;
	t_argument	temp;

	ft_get_redir_state(arg, &redir, state);
	ft_init_redir_variable(&redir);
	ft_sort_redir_command(arg, redir.list_arg, redir.list_com);
	pid = fork_safe();
	if (pid == 0)
	{
		redir.pipes = pipes;
		if (ft_set_redir(&redir, redir.redir_arg) == true)
		{
			temp = ft_get_sorted_command(redir.redir_arg, arg);
			ft_execute(&temp, false);
		}
	}
	pipes->current_idx++;
	ft_free_redir_variable(&redir);
	return (ft_redir_re(arg, state, pipes, pid));
}

pid_t	ft_redir_re(t_argument **arg, int state, t_pipes *pipes, int pid)
{
	enum e_token_type	token;

	while (*arg != NULL && ft_is_redir((*arg)->next_token_type) == true)
		*arg = (*arg)->next;
	token = (*arg)->next_token_type;
	if (token == PIPE)
	{
		*arg = (*arg)->next;
		if (state == INIT)
			state = PIPE_START;
		else if (state == PIPE_START)
			state = PIPE_MIDDLE;
		if (ft_is_redir((*arg)->next_token_type) == false)
		{
			if ((*arg)->next_token_type == EOL)
				state = PIPE_END;
			return (ft_execute_pipe(arg, state, pipes));
		}
		else
			return (ft_execute_redir(arg, state, pipes));
	}
	else if (token == EOL)
		*arg = (*arg)->next;
	return (pid);
}
