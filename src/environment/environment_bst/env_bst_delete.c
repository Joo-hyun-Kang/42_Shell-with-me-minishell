/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_bst_delete.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 09:14:46 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/25 15:50:07 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	ft_env_delete_if_both_null(t_env_root *root, t_env *cur_node);
static void	ft_env_delete_if_left_null(t_env_root *root, t_env *cur_node);
static void	ft_env_delete_if_right_null(t_env_root *root, t_env *cur_node);
static void	ft_env_delete_if_not_null(t_env_root *root, t_env *cur, t_env *rep);

void	ft_env_delete(t_env_root *root, char *key)
{
	t_env	*cur_node;
	t_env	*replace_node;

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
	{
		replace_node = cur_node->right;
		while (replace_node->left)
			replace_node = replace_node->left;
		ft_env_delete_if_not_null(root, cur_node, replace_node);
	}
	--(root->size);
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
		cur_node->right->parent = cur_node->parent;
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
		cur_node->left->parent = cur_node->parent;
	}
}

static void	ft_env_delete_if_not_null(t_env_root *root, t_env *cur, t_env *rep)
{
	if (rep->parent->left == rep)
		rep->parent->left = NULL;
	else
		rep->parent->right = NULL;
	if (cur->parent == NULL)
		root->root = rep;
	else
	{
		if (cur->parent->left == cur)
			cur->parent->left = rep;
		else
			cur->parent->right = rep;
	}
	rep->parent = cur->parent;
	if (cur->right == rep)
		rep->left = cur->left;
	else
	{
		rep->left = cur->left;
		rep->right = cur->right;
	}
}
