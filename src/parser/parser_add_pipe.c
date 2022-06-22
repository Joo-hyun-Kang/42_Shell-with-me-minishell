/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_add_pipe.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 04:37:37 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/23 00:12:07 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	signal_add_pipe(int sig);
static void	ft_add_pipe_child(t_token *cur_token, t_env_root *env);

static void	signal_add_pipe(int sig)
{
	(void)sig;
	exit(1);
}

t_token	*ft_add_pipe(t_token *cur_token, t_env_root *env)
{
	t_token	*add;
	int		pid;
	int		status;
	char	*read_line;

	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == 0)
		ft_add_pipe_child(cur_token, env);
	wait(&status);
	if ((status / 256) == 1)
	{
		g_exit = status % 256;
		unlink(ADD_TMP);
		return (NULL);
	}
	else if ((status / 256) == 2)
	{
		printf("minishell: syntax error: unexpected end of file\n");
		g_exit = 258;
		return (NULL);
	}
	else if ((status / 256) == 3)
	{
		printf("Can't open file!\n");
		return (NULL);
	}

	int fd = open(ADD_TMP, O_RDONLY);
	char	*buf = (char *)malloc(sizeof(char) * 21);
	buf[20] = 0;;
	read_line = 0;
	while ((read(fd, buf, 20)) > 0)
	{
		read_line = ft_merge_str(read_line, ft_strdup(buf));
		ft_memset(buf, 0, 20);
	}
	add = ft_tokenizer(read_line, env);
	cur_token->next = add;
	return (cur_token->next);

}

static void	ft_add_pipe_child(t_token *cur_token, t_env_root *env)
{
	char	*read_line;
	int		fd;

	signal(SIGINT, signal_add_pipe);
	read_line = readline("> ");
	if (read_line == NULL)
		exit (2);
	fd = open(ADD_TMP, (O_CREAT | O_TRUNC | O_RDWR), 0666);
	if (fd < 0)
		exit (3);
	ft_putstr_fd(read_line, fd);
	free(read_line);
	close(fd);
	exit(0);
}
