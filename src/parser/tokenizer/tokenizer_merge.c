/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_merge.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:51:20 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/21 16:45:58 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	ft_merge_env(t_lexer *lexer, char **dst, t_env_root *env)
{
	char	*s_pos;
	char	*key;
	char	*value;
	t_env	*node;

	ft_read_lexer(lexer);
	if (ft_cur_char(lexer) == '?' || ft_isdigit(ft_cur_char(lexer)) != false)
	{
		if (ft_cur_char(lexer) == '?')
			*dst = ft_merge_str(*dst, ft_itoa(g_exit));
		ft_read_lexer(lexer);
		return ;
	}
	s_pos = ft_cur_ptr(lexer);
	while (ft_strchr(M_SEP, ft_cur_char(lexer)) == NULL
		&& ft_cur_char(lexer) != '\n'
		&& ft_cur_char(lexer) != '=')
		ft_read_lexer(lexer);
	if ((ft_cur_ptr(lexer) - s_pos) > 0)
	{
		key = ft_strndup(s_pos, ft_cur_ptr(lexer) - s_pos);
		node = ft_env_search(env, key);
		if (node != NULL)
			*dst = ft_merge_str(*dst, ft_strdup(node->pa_value));
	}
	else
		*dst = ft_merge_str(*dst, ft_strdup("$"));
}

void	ft_merge_home(t_lexer *lexer, char **dst, t_env_root *env)
{
	t_env	*node;

	ft_read_lexer(lexer);
	node = ft_env_search(env, ft_strdup("HOME"));
	*dst = ft_merge_str(*dst, ft_strdup(node->pa_value));
}

void	ft_merge_quote(t_lexer *lexer, char **dst, t_env_root *env, char quote)
{
	char	*s_pos;

	s_pos = ft_cur_ptr(lexer);
	while (ft_cur_char(lexer) != '\0')
	{
		if (ft_cur_char(lexer) == quote)
		{
			ft_save_str(lexer, dst, s_pos);
			ft_read_lexer(lexer);
			return ;
		}
		else if (quote == '"' && ft_cur_char(lexer) == M_ENV)
		{
			ft_save_str(lexer, dst, s_pos);
			ft_merge_env(lexer, dst, env);
			s_pos = ft_cur_ptr(lexer);
		}
		else if (ft_cur_char(lexer) == M_BSLASH)
		{
			ft_read_lexer(lexer);
			*dst = ft_merge_str(*dst, \
					ft_strndup(ft_cur_ptr(lexer), 1));
			ft_read_lexer(lexer);
			s_pos = ft_cur_ptr(lexer);
		}
		else
			ft_read_lexer(lexer);
	}
	ft_save_str(lexer, dst, s_pos);
	*dst = ft_merge_str(*dst, ft_strdup("\n"));
	ft_no_quote(lexer, dst, env, quote);
}

void	ft_no_quote(t_lexer *lexer, char **dst, t_env_root *env, char quote)
{
	char	*read_line;
	char	*tmp;

	read_line = readline("> ");
	// FIXME: 예외처리
	if (read_line == NULL)
	{
		printf("minishell: unexpected EOF while looking for matching `\"'\nminishell: syntax error: unexpected end of file\n");
		g_exit = 258;
		return ;
	}
	tmp = ft_strjoin(lexer->cmd_str, read_line);
	free(read_line);
	ft_replace_lexer_cmd_str(lexer, tmp);
	ft_merge_quote(lexer, dst, env, quote);
}

void	ft_save_str(t_lexer *lexer, char **dst, char *s_pos)
{
	if ((ft_cur_ptr(lexer) - s_pos) < 1)
		return ;
	*dst = ft_merge_str(*dst, ft_strndup(s_pos, ft_cur_ptr(lexer) - s_pos));
}
