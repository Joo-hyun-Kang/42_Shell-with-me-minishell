/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_bst_search.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 09:15:37 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/17 09:15:37 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_env	*ft_env_search_recursive(t_env *cur_node, char *key)
{
	if (cur_node == NULL)
		return (NULL);
	if (ft_strcmp(key, cur_node->pa_key) < 0)
		return (ft_env_search_recursive(cur_node->left, key));
	else if (ft_strcmp(key, cur_node->pa_key) > 0)
		return (ft_env_search_recursive(cur_node->right, key));
	return (cur_node);
}

// Key will be Automatically freed.
t_env	*ft_env_search(t_env_root *root, char *key)
{
	t_env	*find_node;

	if (root->root == NULL)
		return (NULL);
	find_node = ft_env_search_recursive(root->root, key);
	free(key);
	return (find_node);
}
