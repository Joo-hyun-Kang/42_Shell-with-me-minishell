/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_cpy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 07:22:45 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/24 20:34:59 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	ft_bstenv_to_dpenv_recursive(t_env *node, char **env, int *idx);

t_env_root	*ft_dpenv_to_bstenv(char **env)
{
	t_env_root	*root;
	char		*key;
	char		*value;

	root = ft_init_env_root();
	while (*env != NULL)
	{
		if (ft_is_dictionary(*env) == 1)
		{
			key = ft_extract_key_from_str(*env);
			value = ft_extract_value_from_str(*env);
			ft_env_insert(root, key, value);
		}
		++env;
	}
	return (root);
}

void	print_env(char **env)
{
	for (int i = 0; env[i] != NULL; ++i)
	{
		printf("%s\n", env[i]);
	}
}

char	**ft_bstenv_to_dpenv(t_env_root *root)
{
	char	**env;
	int		idx;

	if (root->size == 0)
		return (NULL);
	env = (char **)malloc(sizeof(char *) * (root->size + 1));
	if (env == NULL)
		ft_system_err(FAILED_MALLOC);
	env[root->size] = 0;
	idx = 0;
	ft_bstenv_to_dpenv_recursive(root->root, env, &idx);
	print_env(env);
	return (env);
}

void	ft_remove_copy_env(char **env)
{
	int	idx;

	idx = 0;
	while (env[idx] != NULL)
	{
		free(env[idx]);
		++idx;
	}
	free(env);
}

static void	ft_bstenv_to_dpenv_recursive(t_env *node, char **env, int *idx)
{
	if (node == NULL)
		return ;
	ft_bstenv_to_dpenv_recursive(node->left, env, idx);
	env[*idx] = NULL;
	env[*idx] = ft_merge_str(strdup(node->pa_key), ft_strdup("="));
	env[*idx] = ft_merge_str(env[*idx], ft_strdup(node->pa_value));
	++(*idx);
	ft_bstenv_to_dpenv_recursive(node->right, env, idx);
}
