/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 13:37:42 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/23 02:45:00 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/get_next_line.h"

size_t	gnl_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		++i;
	return (i);
}

char	*gnl_find_newline(const char *str)
{
	if (!str)
		return (0);
	while (*str)
	{
		if (*str == '\n')
			return ((char *)str);
		++str;
	}
	return (0);
}
