/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jokang <jokang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 12:08:42 by jokang            #+#    #+#             */
/*   Updated: 2022/07/04 15:56:18 by jokang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_copy_origin_command(t_argument **arg, t_lst *list_com)
{
	int	len;
	int	i;

	len = ft_get_length_2d_arr((*arg)->pa_argument);
	i = 0;
	while (i < len)
	{
		add_arraylist(list_com, ft_strdup((*arg)->pa_argument[i]), RE_COM);
		i++;
	}
}

void	ft_get_gt_variable(t_argument *p, t_lst *list_arg, char **gt_file)
{
	int	len;
	int	i;

	len = ft_get_length_2d_arr(p->pa_argument);
	i = 0;
	if (len == 1)
	{
		*gt_file = p->pa_argument[i];
	}
	i++;
	while (i < len)
	{
		add_arraylist(list_arg, ft_strdup(p->pa_argument[i]), NONE);
		i++;
	}
}

void	ft_get_lt_dlt(t_argument *p, t_lst *l_arg, t_lst *l_com, int token)
{
	static int j = 2;
	int	len;
	int	i;

	len = ft_get_length_2d_arr(p->pa_argument);
	i = 0;
	if (token == LT)
	{
		add_arraylist(l_com, ft_strdup(p->pa_argument[i]), (int)LT_OPEN);
		j++;
	}
	else
	{
		add_arraylist(l_com, ft_strdup(p->pa_argument[i]), (int)DLT_OPEN);
		j++;
	}
	i++;
	while (i < len)
	{
		add_arraylist(l_arg, ft_strdup(p->pa_argument[i]), NONE);
		i++;
	}
}

void	ft_sort_redir_command(t_argument **arg, t_lst *l_arg, t_lst *l_com)
{
	t_argument			*p;
	char				*gt_file;
	enum e_token_type	token;

	ft_copy_origin_command(arg, l_com);
	p = *arg;
	gt_file = NULL;
	while (p->next_token_type != PIPE && p->next_token_type != EOL)
	{
		if (p->next_token_type == GT)
		{
			p = p->next;
			ft_get_gt_variable(p, l_arg, &gt_file);
		}
		else if (p->next_token_type == LT || p->next_token_type == DLT)
		{
			token = p->next_token_type;
			p = p->next;
			ft_get_lt_dlt(p, l_arg, l_com, token);
		}
		else
			p = p->next;
	}
	if (gt_file != NULL)
		add_arraylist(l_com, ft_strdup(gt_file), GT_FILE);
}
