/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 16:52:07 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/10 13:40:03 by kanghyki         ###   ########.fr       */
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

char	*ft_get_env(char **env, char *key)
{
	int		i;
	int		j;
	char	*value;

	i = 0;
	value = 0;
	while (env[i])
	{
		if (ft_is_match_key(env[i], key) == 1)
		{
			j = 0;
			while (env[i][j] != '=')
				++j;
			++j;
			return (ft_strdup(&env[i][j]));
		}
		++i;
	}
	return (value);
}

int	ft_is_match_key(char *env, char *key)
{
	while (*env)
	{
		if (*key != *env)
			break ;
		++key;
		++env;
	}
	if (*env == '=')
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

char	*ft_create_dict(char *key, char *value)
{
	const int	key_len = ft_strlen(key);
	const int	value_len = ft_strlen(value);
	char		*str;

	str = (char *)malloc(sizeof(char) * (key_len + value_len + 2));
	if (str == 0)
		return (0);
	ft_strlcpy(str, key, key_len + 1);
	ft_strlcat(str, "=", key_len + 2);
	ft_strlcat(str, value, key_len + value_len + 2);
	return (str);
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
	new_env[i] = new_dict;
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

char	**ft_unset_env(char **env, char *key)
{
	char	**find_env;
	char	**new_env;

	if (ft_find_matched_key(env, key) != 0)
	{
		new_env = ft_remove_dict_from_env(env, key);
		ft_delete_env(env);
		return (new_env);
	}
	return (env);
}

char	**ft_set_env(char **env, char *key, char *value)
{
	char	**find_env;
	char	**new_env;
	char	*new_dict;

	new_dict = ft_create_dict(key, value);
	find_env = ft_find_matched_key(env, key);
	if (find_env != 0)
	{
		free(*find_env);
		*find_env = new_dict;
		return (env);
	}
	new_env = ft_add_dict_to_env(env, new_dict);
	ft_delete_env(env);
	return (new_env);
}
