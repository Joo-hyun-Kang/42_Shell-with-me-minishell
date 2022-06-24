/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   align_arg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 03:09:29 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/24 16:20:57 by kanghyki         ###   ########.fr       */
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
	new_str = (char **)malloc(sizeof(char *) * (str_size + 2));
	if (new_str == NULL)
		ft_system_err(FAILED_MALLOC);
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

	if (next->pa_argument != NULL)
		next_size = p_strdlen(next->pa_argument);
	if (next_size < 1)
		return ;
	new_str = (char **)malloc(sizeof(char *) * 2);
	if (new_str == NULL)
		ft_system_err(FAILED_MALLOC);
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
	while (arg->next_token_type != EOL)
	{
		if (arg->next_token_type == LT
			|| arg->next_token_type == DLT || arg->next_token_type == GT)
			p_swap_arg(arg, arg->next);
		arg = arg->next;
	}
	return (head);
}
