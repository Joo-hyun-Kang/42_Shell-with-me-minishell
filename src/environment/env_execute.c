/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 14:19:31 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/17 09:09:21 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_print_env_inorder(t_env *root_node, char *str)
{
	if (root_node == NULL)
		return ;
	ft_print_env_inorder(root_node->left, str);
	printf("%s%s=\"%s\"\n", str, root_node->pa_key, root_node->pa_value);
	ft_print_env_inorder(root_node->right, str);
}

void	ft_execute_export(t_argument *arg)
{
	const int	ARG = 1;
	char		*key;
	char		*value;

	if (arg->pa_argument[ARG] == NULL)
		ft_print_env_inorder(arg->env->root, "declare -x ");
	else
	{
		if (ft_is_dictionary(arg->pa_argument[ARG]) == 1)
		{
			key = ft_extract_key_from_str(arg->pa_argument[ARG]);
			value = ft_extract_value_from_str(arg->pa_argument[ARG]);
			printf("key=%s, value=%s\n", key, value);
			ft_env_insert(arg->env, key, value);
		}
	}
}

void	ft_execute_env(t_argument *arg)
{
	ft_print_env_inorder(arg->env->root, "");
}

void	ft_execute_unset(t_argument *arg)
{
	const int	ARG = 1;
	char		*key;
	int			i;

	if (arg->pa_argument[ARG] == NULL)
		return ;
	key = arg->pa_argument[ARG];
	if (key == 0)
		return ;
	i = 0;
	while (key[i])
	{
		if (ft_isalnum(key[i]) == 0)
		{
			printf("minishell: unset: `%s': not a valid identifier\n", key);
			return ;
		}
		++i;
	}
	ft_env_delete(arg->env, key);
}

void	ft_env_simple_command_test(t_argument *arg)
{
	const int	cmd_pos = 0;

	if (arg->pa_argument[cmd_pos] != 0)
	{
		if (ft_strcmp(arg->pa_argument[cmd_pos], "export") == 0)
			ft_execute_export(arg);
		else if (ft_strcmp(arg->pa_argument[cmd_pos], "env") == 0)
			ft_execute_env(arg);
		else if (ft_strcmp(arg->pa_argument[cmd_pos], "unset") == 0)
			ft_execute_unset(arg);
		else if (ft_strcmp(arg->pa_argument[cmd_pos], "exit") == 0)
			exit(1);
	}
}
