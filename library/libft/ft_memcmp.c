/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 03:41:41 by kanghyki          #+#    #+#             */
/*   Updated: 2022/05/30 19:15:51 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (!n)
		return (0);
	while (i + 1 < n)
	{
		if (((unsigned char *)s1)[i] != ((unsigned char *)s2)[i])
			break ;
		++i;
	}
	return (((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
}
