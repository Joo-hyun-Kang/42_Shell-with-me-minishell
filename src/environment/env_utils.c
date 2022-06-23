/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jokang <jokang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 17:57:29 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/24 00:19:53 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	ft_is_dictionary(char *str)
{
	while (*str != '\0')
	{
		if (*str == '=')
			return (1);
		++str;
	}
	return (0);
}

char	*ft_extract_key_from_str(char *str)
{
	char	*s_pos;

	s_pos = str;
	while (*str != '\0' && *str != '=')
		++str;
	return (ft_strndup(s_pos, str - s_pos));
}

char	*ft_extract_value_from_str(char *str)
{
	while (*str != '\0' && *str != '=')
		++str;
	++str;
	return (ft_strdup(str));
}

void	ft_free_env_node(t_env *node)
{
	free(node->pa_key);
	free(node->pa_value);
	free(node);
}

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

void	ft_copy_bstenv_recursive(t_env *node, char **env, int *idx)
{
	if (node->left != NULL)
		ft_copy_bstenv_recursive(node->left, env, idx);
	env[*idx] = strdup(node->pa_key);
	env[*idx] = ft_merge_str(env[*idx], ft_strdup("="));
	env[*idx] = ft_merge_str(env[*idx], ft_strdup(node->pa_value));
	++(*idx);
	if (node->right != NULL)
		ft_copy_bstenv_recursive(node->right, env, idx);
}

char	**ft_copy_bstenv(t_env_root *root)
{
	char	**env;
	int		idx;

	if (root->size == 0)
		return (NULL);
	env = (char **)malloc(sizeof(char *) * (root->size + 1));
	if (env == NULL)
		return (0);
	env[root->size] = 0;
	idx = 0;
	ft_copy_bstenv_recursive(root->root, env, &idx);
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

void	ft_print_copy_env(char **env)
{
	int	idx;

	idx = 0;
	while (env[idx] != NULL)
	{
		printf("%s\n", env[idx]);
		++idx;
	}
}
