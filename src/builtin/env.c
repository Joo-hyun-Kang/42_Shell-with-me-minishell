/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 02:38:16 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/24 08:33:29 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	ft_execute_env_print_node(t_env *root_node);

void	ft_execute_env(t_argument *arg, int is_parent)
{
	ft_execute_env_print_node(arg->env->root);
	if (is_parent == false)
		exit(0);
}

static void	ft_execute_env_print_node(t_env *root_node)
{
	if (root_node == NULL)
		return ;
	ft_execute_env_print_node(root_node->left);
	printf("%s=%s\n", root_node->pa_key, root_node->pa_value);
	ft_execute_env_print_node(root_node->right);
}
