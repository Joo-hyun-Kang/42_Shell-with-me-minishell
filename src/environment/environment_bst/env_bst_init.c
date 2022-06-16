#include "../../../include/minishell.h"

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
