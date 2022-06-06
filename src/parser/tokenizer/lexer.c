/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 02:16:42 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/06 15:55:45 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_lexer	*ft_init_lexer(char *str)
{
	t_lexer	*l;

	l = (t_lexer *)malloc(sizeof(t_lexer));
	if (l == 0)
		return (0);
	l->pa_str = str;
	l->read_pos = 0;
	l->ch = *str;
	l->head_token = 0;
	return (l);
}

void	ft_analyze_string(t_lexer *out_l)
{
	t_token_lst		*t ;
	t_token_lst		*iter;

	t = ft_create_next_token(out_l);
	out_l->head_token = t;
	iter = out_l->head_token;
	t = ft_create_next_token(out_l);
	while (t != 0)
	{
		iter->next = t;
		iter = iter->next;
		t = ft_create_next_token(out_l);
	}
}
