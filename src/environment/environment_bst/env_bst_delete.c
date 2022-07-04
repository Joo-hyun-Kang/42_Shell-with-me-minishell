/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_bst_delete.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 09:14:46 by kanghyki          #+#    #+#             */
/*   Updated: 2022/07/04 16:50:40 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	ft_env_delete_if_both_null(t_env_root *root, t_env *cur_node);
static void	ft_env_delete_if_left_null(t_env_root *root, t_env *cur_node);
static void	ft_env_delete_if_right_null(t_env_root *root, t_env *cur_node);
static void	ft_env_delete_if_not_null(t_env_root *root, t_env *cur);

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
		ft_env_delete_if_not_null(root, cur_node);
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

static void	ft_env_delete_if_not_null(t_env_root *root, t_env *del_node)
{
	t_env	*rep = del_node->right;
	while (rep->left != NULL)
		rep = rep->left;

// 부모
	if (del_node->parent == NULL)
		root->root = rep;
	else
	{
		if (del_node->parent->left == del_node)
			del_node->parent->left = rep;
		else
			del_node->parent->right = rep;
	}
	rep->parent = del_node->parent;

// 왼쪽 자식
	rep->left = del_node->left;
	del_node->left->parent = rep;

// 오른쪽 자식
	if ((del_node != rep->right) && (rep->right != NULL))
	{
		rep->right->parent = rep->parent;
		rep->parent->left = rep->right;
	}
}
