/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 17:27:21 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/23 15:07:05 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*p_heredoc(t_argument *arg, t_token *cur_tok)
{
	int		pid;
	int		status;

	pid = fork();
	signal(SIGQUIT, SIG_IGN);
	if (pid == 0)
		p_heredoc_child(arg, cur_tok);
	wait(&status);
	if (status != 0)
		p_heredoc_err(status);
	arg->next_token_type = GT;
	free(cur_tok->pa_str);
	cur_tok->pa_str = ft_strdup(F_HEREDOC);
	signal(SIGQUIT, ft_sigquit);
	return (cur_tok);
}
