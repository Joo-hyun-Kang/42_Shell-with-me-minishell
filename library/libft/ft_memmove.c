/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 03:41:47 by kanghyki          #+#    #+#             */
/*   Updated: 2022/05/30 19:20:36 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	*conv_dest;

	if (!dest && !src)
		return (0);
	conv_dest = (char *)dest;
	if (dest <= src)
	{
		while (n--)
			*conv_dest++ = *(const char *)src++;
	}
	else
	{
		conv_dest += n - 1;
		src += n - 1;
		while (n--)
			*conv_dest-- = *(const char *)src--;
	}
	return (dest);
}
