/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 02:16:42 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/04 13:01:50 by kanghyki         ###   ########.fr       */
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

void	ft_analyze_string(t_lexer_lst *out_l)
{
	t_token_lst		*t ;
	t_token_lst		*iter;

	t = ft_create_next_token(out_l);
	out_l->head_token = t;
	iter = out_l->head_token;
	while ((t = ft_create_next_token(out_l))->t_type != EOL)
	{
		iter->next = t;
		t->prev = iter;
		iter = iter->next;
	}
	iter->next = t;
	t->prev = iter;
}
