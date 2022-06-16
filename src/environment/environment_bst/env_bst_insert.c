#include "../../../include/minishell.h"

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

void	ft_env_replace_value(t_env *node, char *new_value)
{
	free(node->pa_value);
	node->pa_value = new_value;
}
