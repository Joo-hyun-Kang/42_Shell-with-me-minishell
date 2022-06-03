/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 02:16:42 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/03 14:38:54 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

/*
 * ├── t_lexer_lst
 * │   ├── struct s_token_list	*head_token
 */

t_lexer_lst	*ft_init_lexer(char *str)
{
	t_lexer_lst	*l;

	l = (t_lexer_lst *)malloc(sizeof(t_lexer_lst));
	if (l == 0)
		return (0);
	l->str = str;
	l->r_pos = 0;
	l->ch = *str;
	l->next = 0;
	l->prev = 0;
	l->head_token = 0;
	return (l);
}

void	ft_set_lexer(t_lexer_lst *out_l)
{
	t_token_lst	*t ;
	t_token_lst	*iter;

	t = ft_next_token(out_l);
	while (t != 0)
	{
		if (out_l->head_token == 0)
			out_l->head_token = t;
		else
		{
			iter = out_l->head_token;
			while (iter->next)
				iter = iter->next;
			iter->next = t;
			t ->prev = iter;
		}
		t = ft_next_token(out_l);
	}
}
