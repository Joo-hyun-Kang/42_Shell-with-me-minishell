/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_bst.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 17:57:29 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/16 15:11:43 by kanghyki         ###   ########.fr       */
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
	free(node->pa_value);
	node->pa_value = new_value;
}

int	ft_env_insert_recursive(char *key, char *value, t_env *cur_node)
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

// Key will be Automatically freed.
void	ft_env_delete(t_env_root *root, char *key)
{
	t_env	*cur_node;

	cur_node = ft_env_search(root, key);
	if (cur_node == NULL)
		return ;
	if (cur_node->left == NULL && cur_node->right == NULL)
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
	else if (cur_node->left == NULL)
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
	else if (cur_node->right == NULL)
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
	else
	{
		t_env *replace_node = cur_node->right;
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
	ft_free_env_node(cur_node);
}

char	*ft_extract_value_from_node(t_env *node)
{
	return (node->pa_value);
}

int		ft_is_dictionary(char *str)
{
	while (*str != '\0')
	{
		if (*str == '=')
			return (1);
		++str;
	}
	return (0);
}

// ft_is_dictionary == 1 가 선행되어야 함
char	*ft_extract_key_from_str(char *str)
{
	char	*rtn_str;
	char	*s_pos;

	s_pos = str;
	while (*str != '=')
		++str;
	return (ft_strndup(s_pos, str - s_pos));
}

// ft_is_dictionary == 1 가 선행되어야 함
char	*ft_extract_value_from_str(char *str)
{
	char	*rtn_str;

	while (*str != '=')
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
