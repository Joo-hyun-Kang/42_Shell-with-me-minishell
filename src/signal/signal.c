/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 21:48:20 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/23 03:56:28 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_sig_get_extra_line(int sig)
{
	(void)sig;
	exit(1);
}

void	ft_sigint(int sig)
{
	pid_t	pid;

	(void)sig;
	pid = waitpid(-1, NULL, WNOHANG);
	if (pid == -1)
	{
		rl_replace_line("", 0);
		printf("\n");
		rl_on_new_line();
		rl_redisplay();
	}
	else
		printf("\n");
}

void	ft_sigquit(int sig)
{
	pid_t	pid;

	pid = waitpid(-1, NULL, WNOHANG);
	if (pid == -1)
	{
		rl_on_new_line();
		rl_redisplay();
	}
	else
		printf("Quit: %d\n", sig);
}
