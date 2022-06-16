#include "../../../include/minishell.h"

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
