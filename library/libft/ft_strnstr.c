/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 03:42:31 by kanghyki          #+#    #+#             */
/*   Updated: 2022/05/30 19:19:10 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/libft.h"

void	*ft_strnstr(const char *data, const char *src, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (!*src)
		return ((void *)data);
	while (data[i] && i < len)
	{
		j = 0;
		while (data[i + j] == src[j]
			&& i + j < len)
		{
			++j;
			if (src[j] == 0)
				return ((void *)&data[i]);
		}
		++i;
	}
	return (0);
}
