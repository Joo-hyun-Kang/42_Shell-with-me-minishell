/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 02:35:28 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/24 08:32:59 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	ft_execute_export_print_env(t_env *root_node);
static int	ft_check_export_valid(char *key);

void	ft_execute_export(t_argument *arg, int is_parent)
{
	int			idx;
	char		*key;
	char		*value;

	idx = 1;
	if (arg->pa_argument[idx] == NULL)
		ft_execute_export_print_env(arg->env->root);
	else
	{
		while (arg->pa_argument[idx + 1] != NULL)
			++idx;
		if (ft_check_export_valid(arg->pa_argument[idx]) == false)
			return (ft_error(EXPORT_INVALID, arg->pa_argument[idx], is_parent));
		key = ft_extract_key_from_str(arg->pa_argument[idx]);
		value = ft_extract_value_from_str(arg->pa_argument[idx]);
		ft_env_insert(arg->env, key, value);
	}
	if (is_parent == false)
		exit(0);
}

static void	ft_execute_export_print_env(t_env *root_node)
{
	if (root_node == NULL)
		return ;
	ft_execute_export_print_env(root_node->left);
	printf("declare -x %s=\"%s\"\n", root_node->pa_key, root_node->pa_value);
	ft_execute_export_print_env(root_node->right);
}

static int	ft_check_export_valid(char *key)
{
	int	i;

	i = 0;
	while (key[i] != '\0')
	{
		if (i != 0 && key[i] == '=')
			break ;
		if (ft_isalnum(key[i]) == false && key[i] != '_')
			return (0);
		++i;
	}
	return (1);
}
