/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   align_arg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 03:09:29 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/25 18:17:36 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	p_strdlen(char **str)
{
	int	i;

	i = 0;
	while (str[i] != NULL)
		++i;
	return (i);
}

char	**ft_append_str(char **str, char *src)
{
	char		**new_str;
	int			str_size;
	int			idx;

	if (str != NULL)
		str_size = p_strdlen(str);
	else
		str_size = 0;
	new_str = (char **)malloc_safe(sizeof(char *) * (str_size + 2));
	new_str[str_size + 1] = NULL;
	idx = 0;
	while (idx < str_size)
	{
		new_str[idx] = str[idx];
		++idx;
	}
	new_str[idx] = src;
	free(str);
	return (new_str);
}

void	p_swap_arg(t_argument *cur, t_argument *next)
{
	int		next_size;
	char	**new_str;
	int		idx;

	if (next->pa_argument == NULL)
		return ;
	next_size = p_strdlen(next->pa_argument);
	if (next_size < 1)
		return ;
	new_str = (char **)malloc_safe(sizeof(char *) * 2);
	new_str[1] = NULL;
	new_str[0] = next->pa_argument[0];
	idx = 1;
	while (next->pa_argument[idx] != NULL)
	{
		cur->pa_argument = ft_append_str(cur->pa_argument, \
				next->pa_argument[idx]);
		++idx;
	}
	free(next->pa_argument);
	next->pa_argument = new_str;
}

t_argument	*p_align_arg(t_argument *arg)
{
	t_argument	*head;

	if (arg == NULL)
		return (NULL);
	head = arg;
	if (arg->next_token_type == LT
		|| arg->next_token_type == DLT || arg->next_token_type == GT)
		p_swap_arg(arg, arg->next);
	return (head);
}
