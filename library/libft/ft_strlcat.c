/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 03:42:17 by kanghyki          #+#    #+#             */
/*   Updated: 2022/05/30 19:22:00 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	src_size;
	size_t	dest_size;
	size_t	i;

	i = 0;
	dest_size = ft_strlen(dest);
	src_size = ft_strlen(src);
	if (dest_size + 1 < size)
	{
		while (src[i] && dest_size + i + 1 < size)
		{
			dest[dest_size + i] = src[i];
			++i;
		}
		dest[dest_size + i] = 0;
	}
	if (size > dest_size)
		return (src_size + dest_size);
	return (src_size + size);
}
