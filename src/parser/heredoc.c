/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 17:27:21 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/24 10:07:41 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_token	*p_heredoc_err(int status);

t_token	*p_heredoc(t_argument *arg, t_token *cur_tok)
{
	int		pid;
	int		status;

	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid < 0)
		ft_system_err(FAILED_FORK);
	if (pid == 0)
		p_heredoc_child(arg, cur_tok);
	wait(&status);
	signal(SIGQUIT, ft_sigquit);
	if (status != 0)
		return (p_heredoc_err(status));
	arg->next_token_type = GT;
	free(cur_tok->pa_str);
	cur_tok->pa_str = ft_strdup(F_HEREDOC);
	return (cur_tok);
}

static t_token	*p_heredoc_err(int status)
{
	if ((status / 256) == 1)
	{
		g_exit = 1;
		unlink(F_HEREDOC);
	}
	else if ((status / 256) == 3)
		printf("Can't open file!\n");
	return (NULL);
}
