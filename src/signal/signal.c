/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 21:48:20 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/22 17:01:52 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	ft_sigint(int sig);
static void	ft_sigquit(int sig);

void	ft_set_signal(void)
{
	signal(SIGINT, ft_sigint);
	signal(SIGQUIT, ft_sigquit);
}

static void	ft_sigint(int sig)
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

static void	ft_sigquit(int sig)
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
