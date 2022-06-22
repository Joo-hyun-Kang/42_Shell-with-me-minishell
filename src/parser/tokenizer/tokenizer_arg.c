/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_arg.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:51:20 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/23 05:23:17 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	tk_replace_env(t_lexer *lexer)
{
	t_env	*node;

	lx_read(lexer);
	if (lx_chr(lexer) == '?')
	{
		lexer->pa_str = ft_merge_str(lexer->pa_str, ft_itoa(g_exit));
		lx_read(lexer);
		return ;
	}
	lx_set_pos(lexer);
	while (lx_chr(lexer) != '\0'
		&& lx_chr(lexer) != '=' && lx_chr(lexer) != '\n'
		&& ft_strchr(M_SEP, lx_chr(lexer)) == NULL)
		lx_read(lexer);
	if ((lx_ptr(lexer) - lx_get_pos(lexer)) > 0)
	{
		node = ft_env_search(lx_env(lexer), ft_strndup(lx_get_pos(lexer), \
					lx_ptr(lexer) - lx_get_pos(lexer)));
		if (node != NULL)
			lexer->pa_str = ft_merge_str(lexer->pa_str, \
					ft_strdup(node->pa_value));
	}
	else
		lexer->pa_str = ft_merge_str(lexer->pa_str, ft_strdup("$"));
}

void	tk_replace_home(t_lexer *lexer)
{
	t_env	*node;

	lx_read(lexer);
	if (lx_chr(lexer) != '\0' && ft_strchr(M_SPACE, lx_chr(lexer)) != NULL)
	{
		node = ft_env_search(lx_env(lexer), ft_strdup("HOME"));
		if (node != NULL)
			lexer->pa_str = ft_merge_str(lexer->pa_str, \
					ft_strdup(node->pa_value));
	}
	else
		lexer->pa_str = ft_merge_str(lexer->pa_str, ft_strdup("~"));
}

void	tk_quote(t_lexer *lexer)
{
	char	quote;

	quote = lx_chr(lexer);
	lx_read(lexer);
	lx_set_pos(lexer);
	while (lx_chr(lexer) != '\0')
	{
		if (lx_chr(lexer) == quote)
		{
			lx_store_str(lexer);
			lx_read(lexer);
			return ;
		}
		else if (quote == '"' && lx_chr(lexer) == M_ENV)
		{
			lx_store_str(lexer);
			tk_replace_env(lexer);
			lx_set_pos(lexer);
		}
		else
			lx_read(lexer);
	}
	lexer->err = true;
	printf("minishell: syntax error: unclosed quote\n");
}
