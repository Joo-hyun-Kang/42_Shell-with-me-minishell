/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_free_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 04:34:39 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/15 11:51:44 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_free_token(t_token *token)
{
	t_token	*prev;

	prev = token;
	while (token != NULL)
	{
		token = token->next;
		free(prev);
		prev = token;
	}
}

void	ft_free_argument(t_argument *arg)
{
	t_argument	*prev;
	t_argument	*tmp;
	int			i;

	prev = arg;
	while (prev != NULL)
	{
		i = 0;
		while (prev->pa_argument[i] != NULL)
		{
			free(prev->pa_argument[i]);
			++i;
		}
		free(prev->pa_argument);
		tmp = prev->next;
		free(prev);
		prev = tmp;
	}
}
