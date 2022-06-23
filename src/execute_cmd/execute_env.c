/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 14:19:31 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/23 21:30:14 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	ft_check_valid_identifier(char *key);

void	ft_excute_export_print(t_env *root_node, char *str)
{
	if (root_node == NULL)
		return ;
	ft_excute_export_print(root_node->left, str);
	printf("%s%s=\"%s\"\n", str, root_node->pa_key, root_node->pa_value);
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
		if (ft_check_valid_identifier(arg->pa_argument[ARG]) == true)
		{
			key = ft_extract_key_from_str(arg->pa_argument[ARG]);
			value = ft_extract_value_from_str(arg->pa_argument[ARG]);
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
	if (ft_check_valid_identifier(arg->pa_argument[ARG]) == true)
		ft_env_delete(arg->env, ft_strdup(arg->pa_argument[ARG]));
	if (is_parent == false)
		exit(0);
}

static int	ft_check_valid_identifier(char *key)
{
	int	i;

	i = 0;
	while (key[i] != '\0')
	{
		if (i != 0 && key[i] == '=')
			break ;
		if (ft_isalnum(key[i]) == false)
		{
			printf("minishell: unset: `%s': not a valid identifier\n", key);
			g_exit = 1;
			return (0);
		}
		++i;
	}
	return (1);
}
