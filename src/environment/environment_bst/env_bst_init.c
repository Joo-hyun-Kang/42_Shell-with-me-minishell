/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_bst_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 09:15:32 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/24 10:06:31 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_env_root	*ft_init_env_root(void)
{
	t_env_root	*root;

	root = (t_env_root *)malloc(sizeof(t_env_root));
	if (root == NULL)
		ft_system_err(FAILED_MALLOC);
	ft_memset(root, 0, sizeof(t_env_root));
	return (root);
}

t_env	*ft_init_env_node(char *key, char *value)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	if (node == NULL)
		ft_system_err(FAILED_MALLOC);
	ft_memset(node, 0, sizeof(t_env));
	node->pa_key = key;
	node->pa_value = value;
	return (node);
}
