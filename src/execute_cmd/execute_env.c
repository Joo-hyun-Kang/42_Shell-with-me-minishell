/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jokang <jokang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 14:19:31 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/20 15:37:38 by jokang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_excute_export_print(t_env *root_node, char *str)
{
	if (root_node == NULL)
		return ;
	ft_excute_export_print(root_node->left, str);
	printf("declare -x %s=\"%s\"\n", root_node->pa_key, root_node->pa_value);
	ft_excute_export_print(root_node->right, str);
}

void	ft_excute_env_print(t_env *root_node, char *str)
{
	if (root_node == NULL)
		return ;
	ft_excute_env_print(root_node->left, str);
	printf("%s=%s\n", root_node->pa_key, root_node->pa_value);
	ft_excute_env_print(root_node->right, str);
}

void	ft_execute_export(t_argument *arg, int is_parent)
{
	const int	ARG = 1;
	char		*key;
	char		*value;

	if (arg->pa_argument[ARG] == NULL)
		ft_excute_export_print(arg->env->root, "declare -x ");
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
	if (is_parent == false)
		exit(0);
}

void	ft_execute_env(t_argument *arg, int is_parent)
{
	ft_excute_env_print(arg->env->root, "");
	if (is_parent == false)
		exit(0);
}

void	ft_execute_unset(t_argument *arg, int is_parent)
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
	if (is_parent == false)
		exit(0);
}
