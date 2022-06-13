/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 16:01:19 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/13 13:17:57 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_print_env(char **env)
{
	while (*env != 0)
		printf("%s\n", *(env++));
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
	char	*key;

	key = ft_extract_key_from_dict(new_dict);
	find_env = ft_find_match_key(env, key);
	free(key);
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
