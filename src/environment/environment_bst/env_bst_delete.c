/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_bst_delete.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jokang <jokang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 09:14:46 by kanghyki          #+#    #+#             */
/*   Updated: 2022/07/04 16:32:09 by jokang           ###   ########.fr       */
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

	printf("1\n");
	cur_node = ft_env_search(root, key);
		printf("2\n");
	if (cur_node == NULL)
		return ;
		printf("3\n");
	if (cur_node->left == NULL && cur_node->right == NULL)
	{
		ft_env_delete_if_both_null(root, cur_node);
	printf("4\n");
	}
	else if (cur_node->left == NULL)
	{
		ft_env_delete_if_left_null(root, cur_node);
			printf("5\n");
	}
	else if (cur_node->right == NULL)
	{
		ft_env_delete_if_right_null(root, cur_node);
			printf("6\n");
	}
	else
	{
			printf("7\n");
				printf("8\n");
		ft_env_delete_if_not_null(root, cur_node, replace_node);
			printf("9\n");
	}
	--(root->size);
		printf("10\n");
	ft_free_env_node(cur_node);
			printf("11\n");
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
	replace_node = cur_node->right;
	while (replace_node->left)
		replace_node = replace_node->left;
	if (replace_node->parent->left == replace_node)
		replace_node->parent->left = NULL;
	else
		replace_node->parent->right = NULL;
	if (cur->parent == NULL)
	{
		root->root = rep;
	}
	else
	{
		if (cur->parent->left == cur)
		{
			cur->parent->left = rep;
		}
		else
		{
			cur->parent->right = rep;
		}
	}
	rep->parent = cur->parent;
	rep->left = cur->left;
	cur->left->parent = rep;
	if (cur->right != rep)
	{
		if (rep->right != NULL)
		{
			rep->right->parent = rep->parent;
			rep->parent->left = rep->right;
		}
		rep->right = cur->right;
	}
}
