/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_additional.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 04:37:37 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/21 21:40:51 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*ft_get_heredoc(char *heredoc);
static char	*ft_env_heredoc(char *str, t_env_root *env);
static void	ft_replace_env_heredoc(char **str, char **dst, t_env_root *env);

t_token	*ft_additional_pipe(t_token *cur_token, t_env_root *env)
{
	char	*read_line;
	t_token	*add;

	read_line = readline("> ");
	if (read_line == NULL)
	{
		printf("minishell: syntax error: unexpected end of file\n");
		g_exit = 258;
		return (NULL);
	}
	add = ft_tokenizer(read_line, env);
	cur_token->next = add;
	return (cur_token->next);
}

t_token	*ft_heredoc(t_argument *arg, t_token *cur_token)
{
	char	*pa_heredoc;
	char	*pa_env_heredoc;
	int		fd;

	fd = open("doc.here", (O_CREAT | O_TRUNC | O_RDWR), 0666);
	arg->next_token_type = GT;
	pa_heredoc = ft_get_heredoc(cur_token->pa_str);
	free(cur_token->pa_str);
	pa_env_heredoc = ft_env_heredoc(pa_heredoc, arg->env);
	ft_putstr_fd(pa_env_heredoc, fd);
	close(fd);
	free(pa_heredoc);
	free(pa_env_heredoc);
	cur_token->pa_str = ft_strdup(F_HEREDOC);
	return (cur_token);
}

static char	*ft_get_heredoc(char *heredoc)
{
	char	*read_line;
	char	*pa_str;

	pa_str = NULL;
	read_line = readline("> ");
	while (read_line != NULL && ft_strcmp(heredoc, read_line) != 0)
	{
		pa_str = ft_merge_str(pa_str, read_line);
		pa_str = ft_merge_str(pa_str, ft_strdup("\n"));
		read_line = readline("> ");
	}
	if (read_line != NULL)
		free(read_line);
	return (pa_str);
}

static char	*ft_env_heredoc(char *str, t_env_root *env)
{
	char	*rtn_str;
	char	*s_pos;
	t_env	*node;

	rtn_str = NULL;
	s_pos = str;
	while (*str != '\0')
	{
		if (*str == '$')
		{
			rtn_str = ft_merge_str(rtn_str, ft_strndup(s_pos, str - s_pos));
			ft_replace_env_heredoc(&str, &rtn_str, env);
			s_pos = str;
		}
		else
			++str;
	}
	rtn_str = ft_merge_str(rtn_str, ft_strndup(s_pos, str - s_pos));
	return (rtn_str);
}

static void	ft_replace_env_heredoc(char **str, char **dst, t_env_root *env)
{
	char	*s_pos;
	char	*value;
	t_env	*node;

	++(*str);
	if (*(*str) == '?' || ft_isdigit(*(*str)) != false)
	{
		if (*(*str) == '?')
			*dst = ft_merge_str(*dst, ft_itoa(g_exit));
		++(*str);
		return ;
	}
	s_pos = *str;
	while (ft_strchr(M_SEP, *(*str)) == NULL
		&& *(*str) != '\n' && *(*str) != '=')
		++(*str);
	if ((*str - s_pos) > 0)
	{
		node = ft_env_search(env, ft_strndup(s_pos, *str - s_pos));
		if (node != NULL)
			*dst = ft_merge_str(*dst, ft_strdup(node->pa_value));
	}
	else
		*dst = ft_merge_str(*dst, ft_strdup("$"));
}
