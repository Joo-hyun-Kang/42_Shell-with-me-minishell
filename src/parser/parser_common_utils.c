/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_common_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 04:12:34 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/12 04:12:40 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_merge_string(char **dst, char *src)
{
	char	*new_str;

	if (*dst == 0)
		*dst = src;
	else
	{
		new_str = ft_strjoin(*dst, src);
		free(*dst);
		free(src);
		*dst = new_str;
	}
}
