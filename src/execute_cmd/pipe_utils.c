/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jokang <jokang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 02:42:17 by jokang            #+#    #+#             */
/*   Updated: 2022/06/25 17:16:07 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_close_pipe(t_pipes *pipes)
{
	int	i;

	i = 0;
	while (i < pipes->pipe_count)
	{
		close(pipes->array[i][PIPE_READ]);
		close(pipes->array[i][PIPE_WRITE]);
		i++;
	}
}

void	ft_free_pipes(t_pipes **pipes)
{
	t_pipes	*p;
	int		i;

	p = *pipes;
	i = 0;
	while (i < p->pipe_count)
	{
		free(p->array[i]);
		i++;
	}
	free(p->array);
	free(*pipes);
	pipes = NULL;
}

int	ft_get_pipe_count(t_argument *arg)
{
	t_argument	*p;
	int			pipe_count;

	p = arg;
	pipe_count = 0;
	while (p != NULL)
	{
		if (p->next_token_type == PIPE)
			pipe_count++;
		p = p->next;
	}
	return (pipe_count);
}
