/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jokang <jokang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 17:57:29 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/24 07:26:04 by kanghyki         ###   ########.fr       */
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

char	*ft_extract_key_from_str(char *str)
{
	char	*s_pos;

	s_pos = str;
	while (*str != '\0' && *str != '=')
		++str;
	return (ft_strndup(s_pos, str - s_pos));
}

char	*ft_extract_value_from_str(char *str)
{
	while (*str != '\0' && *str != '=')
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
