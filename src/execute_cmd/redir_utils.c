/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jokang <jokang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 16:17:49 by jokang            #+#    #+#             */
/*   Updated: 2022/06/25 16:19:40 by jokang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"

int	ft_is_redir(enum e_token_type token)
{
	if (token == LT)
		return (true);
	else if (token == DLT)
		return (true);
	else if (token == GT)
		return (true);
	else if (token == DGT)
		return (true);
	return (false);
}

int	ft_find_next_pipe(t_argument **arg)
{
	t_argument	*p;

	p = *arg;
	while (p != NULL)
	{
		if (p->next_token_type == PIPE)
			return (true);
		p = p->next;
	}
	return (false);
}

void	ft_free_redir_variable(t_redir *redir)
{
	free_arraylist(redir->list_arg);
	free_arraylist(redir->list_com);
	free_arraylist(redir->redir_arg);
}

void	ft_make_redir_com(t_redir *redir, t_lst *arg)
{
	int	i;

	i = 0;
	while (i < redir->list_arg->length)
	{
		add_arraylist(arg, ft_strdup(redir->list_arg->pa_arr[i]), NONE);
		++i;
	}
}
