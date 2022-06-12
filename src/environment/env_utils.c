/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 16:52:07 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/12 16:59:08 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_strdplen(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		++i;
	return (i);
}

char	**ft_envdup(char **env)
{
	char	**duplicated;
	int		env_len;
	int		i;

	i = 0;
	env_len = ft_strdplen(env);
	duplicated = (char **)malloc(sizeof(char *) * (env_len + 1));
	if (duplicated == 0)
		return (0);
	duplicated[env_len] = 0;
	while (env[i])
	{
		duplicated[i] = ft_strdup(env[i]);
		++i;
	}
	return (duplicated);
}

int	ft_is_key_match(char *env, char *key)
{
	int	i;

	i = 0;
	while (env[i] && key[i])
	{
		if ((env[i] != key[i]) || env[i] == '=')
			break ;
		++i;
	}
	if (env[i] == '=' && (key[i] == '=' || key[i] == 0))
		return (1);
	return (0);
}

char	**ft_find_match_key(char **env, char *key)
{
	int		i;
	char	*value;

	i = 0;
	while (env[i])
	{
		if (ft_is_key_match(env[i], key) == 1)
			return (&env[i]);
		++i;
	}
	return (0);
}

char	*ft_get_value_from_env(char **env, char *key)
{
	int		i;
	char	*value;

	i = 0;
	while (*env)
	{
		if (ft_is_key_match(*env, key) == 1)
		{
			while ((*env)[i] != '=')
				++i;
			++i;
			return (ft_strdup(&(*env)[i]));
		}
		else
			++env;
	}
	return (ft_strdup(""));
}
