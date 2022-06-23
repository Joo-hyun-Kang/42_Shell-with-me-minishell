/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_bst_delete.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 09:14:46 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/23 23:02:17 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	ft_env_delete_if_both_null(t_env_root *root, t_env *cur_node);
static void	ft_env_delete_if_left_null(t_env_root *root, t_env *cur_node);
static void	ft_env_delete_if_right_null(t_env_root *root, t_env *cur_node);
static void	ft_env_delete_if_both_not_null(t_env_root *root, t_env *cur_node);

void	ft_env_delete(t_env_root *root, char *key)
{
	t_env	*cur_node;

	cur_node = ft_env_search(root, key);
	if (cur_node == NULL)
		return ;
	if (cur_node->left == NULL && cur_node->right == NULL)
		ft_env_delete_if_both_null(root, cur_node);
	else if (cur_node->left == NULL)
		ft_env_delete_if_left_null(root, cur_node);
	else if (cur_node->right == NULL)
		ft_env_delete_if_right_null(root, cur_node);
	else
		ft_env_delete_if_both_not_null(root, cur_node);
	--root->size;
	ft_free_env_node(cur_node);
}

static void	ft_env_delete_if_both_null(t_env_root *root, t_env *cur_node)
{
	if (cur_node->parent == NULL)
		root->root = NULL;
	else
	{
		if (cur_node->parent->left == cur_node)
			cur_node->parent->left = NULL;
		else
			cur_node->parent->right = NULL;
	}
}

static void	ft_env_delete_if_left_null(t_env_root *root, t_env *cur_node)
{
	if (cur_node->parent == NULL)
		root->root = cur_node->right;
	else
	{
		if (cur_node->parent->left == cur_node)
			cur_node->parent->left = cur_node->right;
		else
			cur_node->parent->right = cur_node->right;
	}
}

static void	ft_env_delete_if_right_null(t_env_root *root, t_env *cur_node)
{
	if (cur_node->parent == NULL)
		root->root = cur_node->left;
	else
	{
		if (cur_node->parent->left == cur_node)
			cur_node->parent->left = cur_node->left;
		else
			cur_node->parent->right = cur_node->left;
	}
}

static void	ft_env_delete_if_both_not_null(t_env_root *root, t_env *cur_node)
{
	t_env	*replace_node;

	replace_node = cur_node->right;
	while (replace_node->left)
		replace_node = replace_node->left;
	if (replace_node->parent->left == cur_node)
		replace_node->parent->left = NULL;
	else
		replace_node->parent->right = NULL;
	if (cur_node->parent == NULL)
		root->root = replace_node;
	else
	{
		if (cur_node->parent->left == cur_node)
			cur_node->parent->left = replace_node;
		else
			cur_node->parent->right = replace_node;
	}
	replace_node->left = cur_node->left;
	replace_node->right = cur_node->right;
}
