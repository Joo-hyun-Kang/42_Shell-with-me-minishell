/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_bst_search.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jokang <jokang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 09:15:37 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/24 18:08:15 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static t_env	*ft_env_search_recursive(t_env *cur_node, char *key);

t_env	*ft_env_search(t_env_root *root, char *key)
{
	t_env	*find_node;

	if (root->root == NULL)
	{
		free(key);
		return (NULL);
	}
	find_node = ft_env_search_recursive(root->root, key);
	free(key);
	return (find_node);
}

static t_env	*ft_env_search_recursive(t_env *cur_node, char *key)
{
	if (cur_node == NULL)
		return (NULL);
	if (ft_strcmp(key, cur_node->pa_key) < 0)
		return (ft_env_search_recursive(cur_node->left, key));
	else if (ft_strcmp(key, cur_node->pa_key) > 0)
		return (ft_env_search_recursive(cur_node->right, key));
	return (cur_node);
}
