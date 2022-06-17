/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 17:57:29 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/18 04:52:41 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	ft_is_dictionary(char *str)
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
