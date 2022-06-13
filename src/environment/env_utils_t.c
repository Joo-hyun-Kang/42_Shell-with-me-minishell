/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_t.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 13:16:23 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/13 13:39:59 by kanghyki         ###   ########.fr       */
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
	while (env[i] != 0)
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
	while (*env != 0)
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

char	*ft_extract_key_from_dict(char *new_dict)
{
	int	i;

	i = 0;
	while (new_dict[i] != 0 && new_dict[i] != '=')
		++i;
	return (ft_strndup(new_dict, i));
}
