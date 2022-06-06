/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 03:42:00 by kanghyki          #+#    #+#             */
/*   Updated: 2022/04/26 21:47:16 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	size_t	size;
	int		result;

	if (fd < 0 || !s)
		return ;
	size = ft_strlen(s);
	while (size > 0)
	{
		result = write(fd, s, size);
		if (result == -1)
			break ;
		s += result;
		size -= result;
	}
}
