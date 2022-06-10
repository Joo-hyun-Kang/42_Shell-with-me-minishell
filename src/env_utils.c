/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 16:52:07 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/10 17:55:13 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		ft_strdplen(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		++i;
	return (i);
}

char	**ft_copy_env(char **env)
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

int	ft_is_match_key(char *env, char *key)
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

char	**ft_find_matched_key(char **env, char *key)
{
	int		i;
	char	*value;

	i = 0;
	while (env[i])
	{
		if (ft_is_match_key(env[i], key) == 1)
			return (&env[i]);
		++i;
	}
	return (0);
}

void	ft_delete_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		free(env[i]);
		++i;
	}
	free(env);
}

char	**ft_add_dict_to_env(char **env, char *new_dict)
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

char	**ft_remove_dict_from_env(char **env, char *key)
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
		if (ft_is_match_key(*env, key) == 0)
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

// Usage env = ft_unset_env(env, new_dict);
char	**ft_unset_env(char **env, char *key)
{
	char	**new_env;

	if (ft_find_matched_key(env, key) != 0)
	{
		new_env = ft_remove_dict_from_env(env, key);
		ft_delete_env(env);
		return (new_env);
	}
	return (env);
}

// Usage env = ft_set_env(env, new_dict);
char	**ft_set_env(char **env, char *new_dict)
{
	char	**find_env;
	char	**new_env;

	find_env = ft_find_matched_key(env, new_dict);
	if (find_env != 0)
	{
		free(*find_env);
		*find_env = ft_strdup(new_dict);
		return (env);
	}
	new_env = ft_add_dict_to_env(env, new_dict);
	ft_delete_env(env);
	return (new_env);
}

void	ft_print_env(char **env)
{
	int i = 0;
	
	while (env[i])
	{
		printf("%s\n", env[i]);
		++i;
	}
}
