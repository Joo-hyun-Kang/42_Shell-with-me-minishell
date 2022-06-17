/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_core.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 17:15:28 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/18 04:50:43 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	ft_reserved(t_lexer *lexer, t_env_root *env, char **rtn_str);

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

t_token	*ft_create_token_type_argument(t_lexer *lexer, t_env_root *env)
{
	char	*s_pos;
	char	*rtn_str;
	char	quote;

	rtn_str = 0;
	while (ft_cur_char(lexer) != '\0')
	{
		s_pos = ft_cur_ptr(lexer);
		while (ft_strchr(M_SEP, ft_cur_char(lexer)) == NULL)
			ft_read_lexer(lexer);
		rtn_str = ft_merge_str(rtn_str, \
				ft_strndup(s_pos, ft_cur_ptr(lexer) - s_pos));
		ft_reserved(lexer, env, &rtn_str);
		if (ft_strchr_except_null(M_SPACE, ft_cur_char(lexer)) != NULL
			|| ft_strchr_except_null(M_META, ft_cur_char(lexer)) != NULL)
			break ;
	}
	return (ft_init_token(rtn_str, ARGUMENT));
}

static void	ft_reserved(t_lexer *lexer, t_env_root *env, char **rtn_str)
{
	if (ft_strchr_except_null(M_QUOTE, ft_cur_char(lexer)) != NULL)
		ft_merge_quote(lexer, rtn_str, env, ft_read_lexer(lexer));
	else if (ft_cur_char(lexer) == M_HOME)
		ft_merge_home(lexer, rtn_str, env);
	else if (ft_cur_char(lexer) == M_ENV)
		ft_merge_env(lexer, rtn_str, env);
}
