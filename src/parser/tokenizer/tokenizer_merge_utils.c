/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_merge_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 17:00:17 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/23 00:40:09 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	ft_merge_home(t_lexer *lexer)
{
	t_env	*node;

	ft_read_lexer(lexer);
	node = ft_env_search(ft_get_env(lexer), ft_strdup("HOME"));
	lexer->pa_str = ft_merge_str(lexer->pa_str, ft_strdup(node->pa_value));
}

void	ft_save_str(t_lexer *lexer)
{
	char	*spos;

	spos = ft_get_spos(lexer);
	if ((ft_cur_ptr(lexer) - spos) < 1)
		return ;
	lexer->pa_str = ft_merge_str(lexer->pa_str, \
			ft_strndup(spos, ft_cur_ptr(lexer) - spos));
}
