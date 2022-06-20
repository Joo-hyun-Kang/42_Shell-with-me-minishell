/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_bst_insert.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jokang <jokang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 09:14:02 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/20 15:36:23 by jokang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	ft_env_insert_recursive(char *key, char *value, t_env *cur_node);
static void	ft_env_replace_value(t_env *node, char *new_value);

// Unused key will be automatically freed.
void	ft_env_insert(t_env_root *root, char *key, char *value)
{
	if (root->root == NULL)
		root->root = ft_init_env_node(key, value);
	else
	{
		if (ft_env_insert_recursive(key, value, root->root) == 0)
			free(key);
	}
}

static int	ft_env_insert_recursive(char *key, char *value, t_env *cur_node)
{
	if (ft_strcmp(key, cur_node->pa_key) < 0)
	{
		if (cur_node->left == NULL)
		{
			cur_node->left = ft_init_env_node(key, value);
			cur_node->left->parent = cur_node;
			return (1);
		}
		return (ft_env_insert_recursive(key, value, cur_node->left));
	}
	else if (ft_strcmp(key, cur_node->pa_key) > 0)
	{
		if (cur_node->right == NULL)
		{
			cur_node->right = ft_init_env_node(key, value);
			cur_node->right->parent = cur_node;
			return (1);
		}
		return (ft_env_insert_recursive(key, value, cur_node->right));
	}
	ft_env_replace_value(cur_node, value);
	return (0);
}

static void	ft_env_replace_value(t_env *node, char *new_value)
{
	free(node->pa_value);
	node->pa_value = new_value;
}
