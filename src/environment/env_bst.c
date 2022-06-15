/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_bst.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 17:57:29 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/15 19:02:16 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env_root	*ft_init_env_root(void)
{
	t_env_root	*root;

	root = (t_env_root *)malloc(sizeof(t_env_root));
	if (root == NULL)
		return (NULL);
	ft_memset(root, 0, sizeof(t_env_root));
	return (root);
}

t_env	*ft_init_env_node(char *key, char *value)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	if (node == NULL)
		return (NULL);
	ft_memset(node, 0, sizeof(t_env));
	node->pa_key = key;
	node->pa_value = value;
	return (node);
}

void	ft_env_replace_value(t_env *node, char *new_value)
{
	if (node->pa_value != NULL)
		free(node->pa_value);
	node->pa_value = new_value;
}

void	ft_env_insert_recursive(char *key, char *value, t_env *cur_node)
{
	if (ft_strcmp(key, cur_node->pa_key) < 0)
	{
		if (cur_node->left == NULL)
		{
			cur_node->left = ft_init_env_node(key, value);
			cur_node->is_del = 0;
		}
		return (ft_env_insert_recursive(key, value, cur_node->left));
	}
	else if (ft_strcmp(key, cur_node->pa_key) > 0)
	{
		if (cur_node->right == NULL)
		{
			cur_node->right = ft_init_env_node(key, value);
			cur_node->is_del = 0;
		}
		return (ft_env_insert_recursive(key, value, cur_node->right));
	}
	else
	{
		ft_env_replace_value(cur_node, value);
		cur_node->is_del = 0;
		free(key);
	}
}

void	ft_env_insert(t_env_root *root, char *key, char *value)
{
	if (root->root == NULL)
		root->root = ft_init_env_node(key, value);
	else
		ft_env_insert_recursive(key, value, root->root);
}

void	ft_print_env_in_order(t_env *cur_node)
{
	if (cur_node == NULL)
		return ;
	ft_print_env_in_order(cur_node->left);
	if (cur_node->is_del == 0)
		printf("%s=%s\n", cur_node->pa_key, cur_node->pa_value);
	ft_print_env_in_order(cur_node->right);
}

t_env	*ft_env_search_recursive(t_env *cur_node, char *key)
{
	if (ft_strcmp(key, cur_node->pa_key) < 0)
		return (ft_env_search_recursive(cur_node->left, key));
	else if (ft_strcmp(key, cur_node->pa_key) > 0)
		return (ft_env_search_recursive(cur_node->right, key));
	return (cur_node);
}

t_env	*ft_env_search(t_env_root *root, char *key)
{
	if (root->root == NULL)
		return (NULL);
	return (ft_env_search_recursive(root->root, key));
}

void	ft_env_delete(t_env_root *root, char *key)
{
	t_env	*del_node;

	del_node = ft_env_search(root, key);
	if (del_node == NULL)
		return ;
	del_node->is_del = 1;
}
