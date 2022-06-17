/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_additional.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 04:37:37 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/18 04:52:16 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	ft_merge_env_heredoc(char **str, char **dst, t_env_root *env);
static char	*ft_env_heredoc(char *str, t_env_root *env);

t_token	*ft_read_additional_pipe(t_token *cur_token, t_env_root *env)
{
	char	*read_line;
	t_token	*add;

	read_line = readline("> ");
	add = ft_tokenizer(read_line, env);
	free(read_line);
	cur_token->next = add;
	return (cur_token->next);
}

static void	ft_merge_env_heredoc(char **str, char **dst, t_env_root *env)
{
	char	*s_pos;
	char	*key;
	char	*value;
	t_env	*node;

	++(*str);
	if (*(*str) == '?')
	{
		*dst = ft_merge_str(*dst, ft_strdup("127"));
		++(*str);
		return ;
	}
	s_pos = *str;
	while (ft_strchr(M_SEP, *(*str)) == NULL && *(*str) != '\n')
		++(*str);
	key = ft_strndup(s_pos, *str - s_pos);
	node = ft_env_search(env, key);
	if (node == NULL)
		*dst = ft_merge_str(*dst, ft_strdup(""));
	else
		*dst = ft_merge_str(*dst, ft_strdup(node->pa_value));
}

static char	*ft_env_heredoc(char *str, t_env_root *env)
{
	char	*rtn_str;
	char	*s_pos;
	t_env	*node;

	rtn_str = 0;
	s_pos = str;
	while (*str != '\0')
	{
		if (*str == '$')
		{
			rtn_str = ft_merge_str(rtn_str, ft_strndup(s_pos, str - s_pos));
			ft_merge_env_heredoc(&str, &rtn_str, env);
			s_pos = str;
		}
		else
			++str;
	}
	rtn_str = ft_merge_str(rtn_str, ft_strndup(s_pos, str - s_pos));
	return (rtn_str);
}

void	ft_read_additional_heredoc(t_token *cur_token, t_env_root *env)
{
	char	*read_line;
	char	*new_str;

	new_str = 0;
	read_line = readline("> ");
	while (ft_strcmp(cur_token->pa_str, read_line) != 0)
	{
		new_str = ft_merge_str(new_str, read_line);
		new_str = ft_merge_str(new_str, ft_strdup("\n"));
		read_line = readline("> ");
	}
	free(read_line);
	free(cur_token->pa_str);
	read_line = new_str;
	cur_token->pa_str = ft_env_heredoc(new_str, env);
	free(read_line);
}
