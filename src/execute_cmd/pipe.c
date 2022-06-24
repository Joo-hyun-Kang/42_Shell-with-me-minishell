/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jokang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 01:35:04 by jokang            #+#    #+#             */
/*   Updated: 2022/06/24 01:35:05 by jokang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"

void	ft_get_pipe_state(int *state)
{
	if (*state == INIT)
		*state = PIPE_START;
	else if (*state == PIPE_START)
		*state = PIPE_MIDDLE;
	else if (*state == END)
		*state = PIPE_END;
}

pid_t	ft_execute_pipe(t_argument **arg, int state, t_pipes *pipes)
{
	pid_t				pid;
	enum e_token_type	token;

	ft_get_pipe_state(&state);
	pid = fork();
	if (pid == -1)
		printf("error\n");
	else if (pid == 0)
	{
		ft_set_pipe(pipes, state);
		ft_execute(*arg, false);
	}
	pipes->current_idx++;
	*arg = (*arg)->next;
	if (*arg == NULL)
		return pid;
	token = (*arg)->next_token_type;
	if (token == PIPE)
		return ft_execute_pipe(arg, state, pipes);
	else if (ft_is_redir(token))
		return ft_execute_redir(arg, state, pipes);
	else
		return ft_execute_pipe(arg, END, pipes);
	return (pid);
}

int	ft_construt_pipes(t_argument *arg, t_pipes *pipes)
{
	t_pipes		*temp;
	int			pipe_count;
	int			i;
	int			ret;

	pipe_count = ft_get_pipe_count(arg);
	pipes->array = (int **)malloc(sizeof(int *) * pipe_count);
	if (pipes->array == NULL)
		printf("error\n");
	pipes->pipe_count = pipe_count;
	pipes->current_idx = 0;
	i = 0;
	while (i < pipe_count)
	{
		pipes->array[i] = (int *)malloc(sizeof(int) * PIPE_COUNT);
		ret = pipe(pipes->array[i]);
		if (ret < -1)
			printf("error\n");
		i++;
	}
	return (true);
}

void	ft_set_pipe(t_pipes *pipes, int state)
{
	int	*cur_pipe;
	int	*pre_pipe;
	int	i;

	if (state == PIPE_START)
	{
		cur_pipe = pipes->array[pipes->current_idx];
		dup2(cur_pipe[PIPE_WRITE], STDOUT_FILENO);
	}
	else if (state == PIPE_MIDDLE)
	{
		cur_pipe = pipes->array[pipes->current_idx];
		pre_pipe = pipes->array[pipes->current_idx - 1];
		dup2(pre_pipe[PIPE_READ], STDIN_FILENO);
		dup2(cur_pipe[PIPE_WRITE], STDOUT_FILENO);
	}
	else if (state == PIPE_END)
	{
		pre_pipe = pipes->array[pipes->current_idx - 1];
		dup2(pre_pipe[PIPE_READ], STDIN_FILENO);
	}
	ft_close_pipe(pipes);
}
