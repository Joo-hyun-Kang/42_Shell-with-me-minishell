/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 02:16:42 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/03 02:46:23 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

/*
 * ├── t_cmd_lst
 * │   ├── struct s_token_list	*head_token
 */

t_cmd_lst	*ft_init_cmd(char *str)
{
	t_cmd_lst	*cl;

	cl = (t_cmd_lst *)malloc(sizeof(t_cmd_lst));
	if (cl == 0)
		return (0);
	cl->cmd = str;
	cl->r_pos = 0;
	cl->ch = *str;
	cl->next = 0;
	cl->prev = 0;
	cl->head_token = 0;
	return (cl);
}

void	ft_set_cmd_lst(t_cmd_lst *out_cl)
{
	t_token_lst	*token;
	t_token_lst	*iter;

	token = ft_next_token(out_cl);
	while (token != 0)
	{
		if (out_cl->head_token == 0)
			out_cl->head_token = token;
		else
		{
			iter = out_cl->head_token;
			while (iter->next)
				iter = iter->next;
			iter->next = token;
			token->prev = iter;
		}
		token = ft_next_token(out_cl);
	}
}

t_cmd_lst	*ft_get_cmd_lst(char *str)
{
	t_cmd_lst	*cl;

	cl = ft_init_cmd(str);
	if (cl == 0)
		return (0);
	ft_set_cmd_lst(cl);
	return (cl);
}
