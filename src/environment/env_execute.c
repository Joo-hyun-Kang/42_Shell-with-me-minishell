/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 14:19:31 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/14 16:13:11 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] != 0 && s2[i] != 0)
	{
		if (s1[i] != s2[i])
			break ;
		++i;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	ft_execute_export(t_argument *arg)
{
	char	*dict;
	int		i;

	dict = arg->pa_argument[1];
	if (dict == 0)
	{
		ft_print_env(*(arg->env));
		return ;
	}
	i = 0;
	while (dict[i])
	{
		if (i != 0 && dict[i] == '=')
			break ;
		if (ft_isalnum(dict[i]) == 0)
		{
			printf("minishell: unset: `%s': not a valid identifier\n", dict);
			return ;
		}
		++i;
	}
	if (dict != 0)
		*(arg->env) = ft_set_env(*(arg->env), dict);
}

void	ft_execute_env(t_argument *arg)
{
	char	*dict;

	dict = arg->pa_argument[1];
	if (dict != 0)
		*(arg->env) = ft_set_env(*(arg->env), dict);
	ft_print_env(*(arg->env));
}

void	ft_execute_unset(t_argument *arg)
{
	char	*key;
	int		i;

	key = arg->pa_argument[1];
	if (key == 0)
		return ;
	i = 0;
	while (key[i])
	{
		if (ft_isalnum(key[i]) == 0)
		{
			printf("minishell: unset: `%s': not a valid identifier\n", key);
			return ;
		}
		++i;
	}
	*(arg->env) = ft_unset_env(*(arg->env), key);
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
		else if (ft_strcmp(arg->pa_argument[COMMAND_POSITION], "exit") == 0)
			exit(1);
	}
	ft_free_argument(arg);
}
