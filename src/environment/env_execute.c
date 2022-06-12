/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 14:19:31 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/12 17:00:11 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			break ;
		++i;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	ft_execute_export(t_argument *arg)
{
	if (arg->pa_argument[1] != 0)
		*(arg->env) = ft_set_env(*(arg->env), arg->pa_argument[1]);
	else
		ft_print_env(*(arg->env));
}

void	ft_execute_env(t_argument *arg)
{
	if (arg->pa_argument[1] != 0)
		*(arg->env) = ft_set_env(*(arg->env), arg->pa_argument[1]);
	ft_print_env(*(arg->env));
}

void	ft_execute_unset(t_argument *arg)
{
	if (arg->pa_argument[1] != 0)
		*(arg->env) = ft_unset_env(*(arg->env), arg->pa_argument[1]);
}

void	ft_env_simple_command_test(t_argument *arg)
{
	const int	COMMAND_POSITION = 0;

	if (arg->pa_argument[COMMAND_POSITION] != 0)
	{
		if (ft_strcmp(arg->pa_argument[COMMAND_POSITION], "export") == 0)
			ft_execute_export(arg);
		else if (ft_strcmp(arg->pa_argument[COMMAND_POSITION], "env") == 0)
			ft_execute_env(arg);
		else if (ft_strcmp(arg->pa_argument[COMMAND_POSITION], "unset") == 0)
			ft_execute_unset(arg);
	}
	ft_free_argument(arg);
}
