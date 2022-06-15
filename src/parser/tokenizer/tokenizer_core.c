/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_core.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 17:15:28 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/15 23:44:30 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_token	*ft_create_token_type_metachar(t_lexer *lexer)
{
	enum e_token_type	token_type;

	token_type = 0;
	if (ft_cur_char(lexer) == ';')
		token_type = SEMICOLON;
	else if (ft_cur_char(lexer) == '|')
		token_type = PIPE;
	else if (ft_cur_char(lexer) == '<')
	{
		token_type = GT;
		if (ft_next_char(lexer) == '<')
			token_type = DGT;
	}
	else if (ft_cur_char(lexer) == '>')
	{
		token_type = LT;
		if (ft_next_char(lexer) == '>')
			token_type = DLT;
	}
	if (token_type == DLT || token_type == DGT)
		ft_read_lexer(lexer);
	ft_read_lexer(lexer);
	return (ft_init_token(NULL, token_type));
}

void	ft_merge_env_str(t_lexer *lexer, char **dst, t_env_root *root_env)
{
	char	*s_pos;
	char	*key;
	char	*value;
	t_env	*node;

	ft_read_lexer(lexer);
	// $? 구현 (임시)
	if (ft_cur_char(lexer) == '?')
	{
		*dst = ft_merge_str(*dst, ft_strdup("127"));
		ft_read_lexer(lexer);
		return ;
	}
	s_pos = ft_cur_ptr(lexer);
	while (ft_strchr(M_SEP, ft_cur_char(lexer)) == 0 && ft_cur_char(lexer) != '\n')
		ft_read_lexer(lexer);
	key = ft_strndup(s_pos, ft_cur_ptr(lexer) - s_pos);
	node = ft_env_search(root_env, key);
	if (node == NULL)
		*dst = ft_merge_str(*dst, ft_strdup(""));
	else
		*dst = ft_merge_str(*dst, ft_strdup(node->pa_value));
}

void	ft_merge_home_str(t_lexer *lexer, char **dst, t_env_root *root_env)
{
	t_env	*node;

	ft_read_lexer(lexer);
	node = ft_env_search(root_env, "HOME");
	*dst = ft_merge_str(*dst, ft_strdup(node->pa_value));
}

void	ft_merge_quote_str(t_lexer *lexer, char **dst, t_env_root *root_env, char quote)
{
	char	*s_pos;
	char	*read_line;
	char	*tmp;

	s_pos = ft_cur_ptr(lexer);
	while (ft_cur_char(lexer) != '\0')
	{
		if (ft_cur_char(lexer) == quote)
		{
			*dst = ft_merge_str(*dst, ft_strndup(s_pos, ft_cur_ptr(lexer) - s_pos));
			ft_read_lexer(lexer);
			return ;
		}
		else if (quote == '"' && ft_cur_char(lexer) == '$')
			ft_merge_env_str(lexer, dst, root_env);
		else
			ft_read_lexer(lexer);
	}
	*dst = ft_merge_str(*dst, \
			ft_merge_str(ft_strndup(s_pos, ft_cur_ptr(lexer) - s_pos), ft_strdup("\n")));
	read_line = readline("> ");
	tmp = ft_strjoin(lexer->cmd_str, read_line);
	ft_replace_lexer_cmd_str(lexer, tmp);
	ft_merge_quote_str(lexer, dst, root_env, quote);
}

t_token	*ft_create_token_type_argument(t_lexer *lexer, t_env_root *root_env)
{
	char	*s_pos;
	char	*rtn_str;
	char	quote;

	rtn_str = 0;
	while (ft_cur_char(lexer) != '\0')
	{
		s_pos = ft_cur_ptr(lexer);
		while (ft_strchr(M_SEP, ft_cur_char(lexer)) == 0)
			ft_read_lexer(lexer);
		rtn_str = ft_merge_str(rtn_str, ft_strndup(s_pos, ft_cur_ptr(lexer) - s_pos));
		if (ft_strchr_except_null(M_QUOTE, ft_cur_char(lexer)) != 0)
		{
			quote = ft_cur_char(lexer);
			ft_read_lexer(lexer);
			ft_merge_quote_str(lexer, &rtn_str, root_env, quote);
		}
		else if (ft_cur_char(lexer) == M_HOME)
			ft_merge_home_str(lexer, &rtn_str, root_env);
		else if (ft_cur_char(lexer) == M_ENV)
			ft_merge_env_str(lexer, &rtn_str, root_env);
		if (ft_strchr_except_null(M_SPACE, ft_cur_char(lexer)) != 0
			|| ft_strchr_except_null(M_META, ft_cur_char(lexer)) != 0)
			break ;
	}
	return (ft_init_token(rtn_str, ARGUMENT));
}
