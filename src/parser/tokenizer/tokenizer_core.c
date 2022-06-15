/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_core.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 17:15:28 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/15 17:15:49 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_token	*ft_create_token_type_metachar(char **cmd_str)
{
	enum e_token_type	token_type;

	token_type = 0;
	if (*(*cmd_str) == ';')
		token_type = SEMICOLON;
	else if (*(*cmd_str) == '|')
		token_type = PIPE;
	else if (*(*cmd_str) == '<')
	{
		token_type = GT;
		if (*(*cmd_str + 1) == '<')
			token_type = DGT;
	}
	else if (*(*cmd_str) == '>')
	{
		token_type = LT;
		if (*(*cmd_str + 1) == '>')
			token_type = DLT;
	}
	if (token_type == DLT || token_type == DGT)
		++(*cmd_str);
	++(*cmd_str);
	return (ft_init_token(NULL, token_type));
}

void	ft_merge_env_str(char **cmd_str, char **dst, char **env)
{
	char	*s_pos;
	char	*key;
	char	*value;

	++(*cmd_str);
	s_pos = *cmd_str;
	while (ft_strchr(M_SEP, *(*cmd_str)) == 0 && *(*cmd_str) != '\n')
		++(*cmd_str);
	key = ft_strndup(s_pos, *cmd_str - s_pos);
	value = ft_get_value_from_env(env, key);
	free(key);
	*dst = ft_merge_str(*dst, value);
}

void	ft_merge_home_str(char **cmd_str, char **dst, char **env)
{
	++(*cmd_str);
	*dst = ft_merge_str(*dst, ft_get_value_from_env(env, "HOME"));
}

void	ft_merge_quote_str(char **cmd_str, char **dst, char **env, char quote)
{
	char	*s_pos;
	char	*read_line;
	char	*tmp;

	s_pos = *cmd_str;
	while (*(*cmd_str) != '\0')
	{
		if (*(*cmd_str) == quote)
		{
			*dst = ft_merge_str(*dst, ft_strndup(s_pos, *cmd_str - s_pos));
			++(*cmd_str);
			return ;
		}
		else if (quote == '"' && *(*cmd_str) == '$')
			ft_merge_env_str(cmd_str, dst, env);
		else
			++(*cmd_str);
	}
	*dst = ft_merge_str(*dst, \
			ft_merge_str(ft_strndup(s_pos, *cmd_str - s_pos), ft_strdup("\n")));
	read_line = readline("> ");
	tmp = read_line;
	ft_merge_quote_str(&read_line, dst, env, quote);
	free(tmp);
}

t_token	*ft_create_token_type_argument(char **cmd_str, char **env)
{
	char	*s_pos;
	char	*rtn_str;

	rtn_str = 0;
	while (*(*cmd_str) != '\0')
	{
		s_pos = *cmd_str;
		while (ft_strchr(M_SEP, *(*cmd_str)) == 0)
			++(*cmd_str);
		rtn_str = ft_merge_str(rtn_str, ft_strndup(s_pos, *cmd_str - s_pos));
		if (ft_strchr_except_null(M_QUOTE, *(*cmd_str)) != 0)
			ft_merge_quote_str(cmd_str, &rtn_str, env, *((*cmd_str)++));
		else if (*(*cmd_str) == M_HOME)
			ft_merge_home_str(cmd_str, &rtn_str, env);
		else if (*(*cmd_str) == M_ENV)
			ft_merge_env_str(cmd_str, &rtn_str, env);
		if (ft_strchr_except_null(M_SPACE, *(*cmd_str)) != 0
			|| ft_strchr_except_null(M_META, *(*cmd_str)) != 0)
			break ;
	}
	return (ft_init_token(rtn_str, ARGUMENT));
}
