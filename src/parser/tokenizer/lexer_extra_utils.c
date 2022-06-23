/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_extra_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 01:45:02 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/23 01:54:28 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	lx_set_pos(t_lexer *lexer)
{
	lexer->pos = lx_ptr(lexer);
}

char	*lx_get_pos(t_lexer *lexer)
{
	return (lexer->pos);
}

t_env_root	*lx_env(t_lexer *lexer)
{
	return (lexer->env);
}

void	lx_store_str(t_lexer *lexer)
{
	char	*spos;

	spos = lx_get_pos(lexer);
	if ((lx_ptr(lexer) - spos) < 1)
		return ;
	lexer->pa_str = ft_merge_str(lexer->pa_str, \
			ft_strndup(spos, lx_ptr(lexer) - spos));
}
