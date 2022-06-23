/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 04:37:37 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/23 15:07:26 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void		p_extra_pipe_child(void);
static void		p_read_extra_pipe_file(t_token *cur_tok, t_env_root *env);
static t_token	*p_extra_pipe_err(int status);

t_token	*p_extra_pipe(t_token *cur_tok, t_env_root *env)
{
	int		pid;
	int		status;

	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == 0)
		p_extra_pipe_child();
	wait(&status);
	if (status != 0)
		return (p_extra_pipe_err(status));
	p_read_extra_pipe_file(cur_tok, env);
	signal(SIGQUIT, ft_sigquit);
	return (cur_tok->next);
}

static void	p_extra_pipe_child(void)
{
	char	*read_line;
	int		fd;

	signal(SIGINT, ft_sig_get_extra_line);
	read_line = readline("> ");
	while (read_line != NULL)
	{
		if (ft_strlen(read_line) != 0)
			break ;
		free(read_line);
		read_line = readline("> ");
	}
	if (read_line == NULL)
		exit (2);
	fd = open(F_EXTRA, (O_CREAT | O_TRUNC | O_RDWR), 0666);
	if (fd < 0)
		exit (3);
	ft_putstr_fd(read_line, fd);
	free(read_line);
	close(fd);
	exit(0);
}

static void	p_read_extra_pipe_file(t_token *cur_tok, t_env_root *env)
{
	char	*buf;
	int		fd;
	t_token	*extra_tok;

	fd = open(F_EXTRA, O_RDONLY);
	buf = get_next_line(fd);
	close(fd);
	unlink(F_EXTRA);
	extra_tok = ft_tokenizer(buf, env);
	free(cur_tok->next);
	cur_tok->next = extra_tok;
}

static t_token	*p_extra_pipe_err(int status)
{
	if ((status / 256) == 1)
	{
		g_exit = 1;
		unlink(F_EXTRA);
	}
	else if ((status / 256) == 2)
	{
		printf("\033[1A\033[2C");
		printf("minishell: syntax error: unexpected end of file\n");
		g_exit = 258;
	}
	else if ((status / 256) == 3)
		printf("Can't open file!\n");
	return (NULL);
}
