/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 02:39:45 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/24 08:33:14 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	ft_check_unset_vaild(char *key);

void	ft_execute_unset(t_argument *arg, int is_parent)
{
	int			idx;
	char		*key;
	int			i;

	idx = 1;
	if (arg->pa_argument[idx] == NULL)
		return ;
	while (arg->pa_argument[idx + 1] != NULL)
		++idx;
	if (ft_check_unset_vaild(arg->pa_argument[idx]) == false)
		return (ft_error(UNSET_INVALID, arg->pa_argument[idx], is_parent));
	ft_env_delete(arg->env, ft_strdup(arg->pa_argument[idx]));
	if (is_parent == false)
		exit(0);
}

static int	ft_check_unset_vaild(char *key)
{
	int	i;

	i = 0;
	while (key[i] != '\0')
	{
		if (ft_isalnum(key[i]) == false && key[i] != '_')
			return (0);
		++i;
	}
	return (1);
}
