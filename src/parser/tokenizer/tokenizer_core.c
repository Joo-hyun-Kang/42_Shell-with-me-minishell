/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_core.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 17:15:28 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/21 17:37:02 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	ft_merge_arg(t_lexer *lexer);

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

t_token	*ft_create_token_type_argument(t_lexer *lexer)
{
	while (ft_cur_char(lexer) != '\0')
	{
		ft_set_spos(lexer);
		while (ft_strchr(M_SEP, ft_cur_char(lexer)) == NULL)
			ft_read_lexer(lexer);
		ft_save_str(lexer);
		ft_merge_arg(lexer);
		if (ft_strchr_except_null(M_SPACE, ft_cur_char(lexer)) != NULL
			|| ft_strchr_except_null(M_META, ft_cur_char(lexer)) != NULL)
			break ;
	}
	if (lexer->pa_str == NULL)
		return (NULL);
	return (ft_init_token(lexer->pa_str, ARGUMENT));
}

static void	ft_merge_arg(t_lexer *lexer)
{
	char	quote;

	if (ft_strchr_except_null(M_QUOTE, ft_cur_char(lexer)) != NULL)
	{
		quote = ft_cur_char(lexer);
		ft_read_lexer(lexer);
		ft_merge_quote(lexer, quote);
	}
	else if (ft_cur_char(lexer) == M_HOME)
		ft_merge_home(lexer);
	else if (ft_cur_char(lexer) == M_ENV)
		ft_merge_env(lexer);
	else if (ft_cur_char(lexer) == M_BSLASH)
		ft_merge_bslash(lexer);
}
