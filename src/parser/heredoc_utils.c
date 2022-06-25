/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 03:25:58 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/25 17:18:28 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	p_read_heredoc(char *heredoc, t_argument *arg);

void	p_heredoc_child(t_argument *arg, t_token *cur_tok)
{
	signal(SIGINT, ft_sig_for_child);
	p_read_heredoc(cur_tok->pa_str, arg);
	exit(0);
}

static void	p_read_heredoc(char *heredoc, t_argument *arg)
{
	char	*read_line;
	char	*pa_str;
	char	*write_line;
	int		fd;

	fd = open(F_HEREDOC, (O_CREAT | O_TRUNC | O_RDWR), 0644);
	if (fd < 0)
		exit(3);
	pa_str = NULL;
	read_line = readline("> ");
	while (read_line != NULL && ft_strcmp(heredoc, read_line) != 0)
	{
		pa_str = ft_merge_str(pa_str, read_line);
		pa_str = ft_merge_str(pa_str, ft_strdup("\n"));
		write_line = p_env_heredoc(pa_str, arg->env);
		free(pa_str);
		pa_str = 0;
		ft_putstr_fd(write_line, fd);
		read_line = readline("> ");
	}
	if (read_line != NULL)
		free(read_line);
	close(fd);
}

char	*p_env_heredoc(char *str, t_env_root *env)
{
	char	*rtn_str;
	char	*spos;

	rtn_str = NULL;
	spos = str;
	while (*str != '\0')
	{
		if (*str == '$')
		{
			rtn_str = ft_merge_str(rtn_str, ft_strndup(spos, str - spos));
			p_replace_env_heredoc(&str, &rtn_str, env);
			spos = str;
		}
		else
			++str;
	}
	rtn_str = ft_merge_str(rtn_str, ft_strndup(spos, str - spos));
	return (rtn_str);
}

void	p_replace_env_heredoc(char **str, char **dst, t_env_root *env)
{
	char	*spos;
	t_env	*node;

	++(*str);
	if (*(*str) == '?')
	{
		*dst = ft_merge_str(*dst, ft_itoa(g_exit));
		++(*str);
		return ;
	}
	spos = *str;
	while (*(*str) != '\0' && *(*str) != '=' && *(*str) != '\n'
		&& ft_strchr(M_SEP, *(*str)) == NULL)
		++(*str);
	if ((*str - spos) > 0)
	{
		node = ft_env_search(env, ft_strndup(spos, *str - spos));
		if (node != NULL)
			*dst = ft_merge_str(*dst, ft_strdup(node->pa_value));
	}
	else
		*dst = ft_merge_str(*dst, ft_strdup("$"));
}
