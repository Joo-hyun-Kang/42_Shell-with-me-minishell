/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_bst_delete.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 09:14:46 by kanghyki          #+#    #+#             */
/*   Updated: 2022/07/04 17:2 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	ft_env_delete_if_both_null(t_env_root *root, t_env *cur_node);
static void	ft_env_delete_if_left_null(t_env_root *root, t_env *right);
static void	ft_env_delete_if_right_null(t_env_root *root, t_env *left);
static void	ft_env_delete_if_not_null(t_env_root *root, t_env *cur, t_env *rep);

void	ft_env_delete(t_env_root *root, char *key)
{
	t_env	*cur_node;

	cur_node = ft_env_search(root, key);
	if (cur_node == NULL)
		return ;
	if (cur_node->left == NULL && cur_node->right == NULL)
		ft_env_delete_if_both_null(root, cur_node);
	else if (cur_node->left == NULL)
		ft_env_delete_if_left_null(root, cur_node->right);
	else if (cur_node->right == NULL)
		ft_env_delete_if_right_null(root, cur_node->left);
	else
		ft_env_delete_if_not_null(root, cur_node, cur_node->right);
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

static void	ft_env_delete_if_left_null(t_env_root *root, t_env *right)
{
	t_env	*grandp;

	grandp = right->parent->parent;
	if (grandp == NULL)
		root->root = right;
	else
	{
		if (grandp->right == right->parent)
			grandp->right = right;
		else
			grandp->left = right;
	}
	right->parent = grandp;
}

static void	ft_env_delete_if_right_null(t_env_root *root, t_env *left)
{
	t_env	*grandp;

	grandp = left->parent->parent;
	if (grandp == NULL)
		root->root = left;
	else
	{
		if (grandp->left == left->parent)
			grandp->left = left;
		else
			grandp->right = left;
	}
	left->parent = grandp;
}

static void	ft_env_delete_if_not_null(t_env_root *root, t_env *del, t_env *min)
{
	t_env	*old_parent;

	while (min->left != NULL)
		min = min->left;
	if (del->parent == NULL)
		root->root = min;
	else
	{
		if (del->parent->left == del)
			del->parent->left = min;
		else
			del->parent->right = min;
	}
	old_parent = min->parent;
	min->parent = del->parent;
	min->left = del->left;
	min->left->parent = min;
	if (del->right != min)
	{
		old_parent->left = min->right;
		if (min->right != NULL)
			min->right->parent = old_parent;
		min->right = del->right;
		min->right->parent = min;
	}
}
