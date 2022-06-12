/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 16:01:19 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/12 16:58:49 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**ft_add_env(char **env, char *new_dict)
{
	const int	env_len = ft_strdplen(env);
	char		**new_env;
	int			i;

	new_env = (char **)malloc(sizeof(char *) * (env_len + 2));
	if (new_env == 0)
		return (0);
	new_env[env_len + 1] = 0;
	i = 0;
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		++i;
	}
	new_env[i] = ft_strdup(new_dict);
	return (new_env);
}

char	**ft_remove_env(char **env, char *key)
{
	const int	env_len = ft_strdplen(env);
	char		**new_env;
	int			i;

	new_env = (char **)malloc(sizeof(char *) * (env_len));
	if (new_env == 0)
		return (0);
	new_env[env_len - 1] = 0;
	i = 0;
	while (*env)
	{
		if (ft_is_key_match(*env, key) == 0)
		{
			new_env[i] = ft_strdup(*env);
			++env;
			++i;
		}
		else
			++env;
	}
	return (new_env);
}

void	ft_print_env(char **env)
{
	while (*env)
	{
		printf("%s\n", *env);
		++env;
	}
}

char	**ft_unset_env(char **env, char *key)
{
	char	**new_env;

	if (ft_find_match_key(env, key) != 0)
	{
		new_env = ft_remove_env(env, key);
		ft_free_env(env);
		return (new_env);
	}
	return (env);
}

char	**ft_set_env(char **env, char *new_dict)
{
	char	**find_env;
	char	**new_env;

	find_env = ft_find_match_key(env, new_dict);
	if (find_env != 0)
	{
		free(*find_env);
		*find_env = ft_strdup(new_dict);
		return (env);
	}
	new_env = ft_add_env(env, new_dict);
	ft_free_env(env);
	return (new_env);
}
