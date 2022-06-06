/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 03:41:53 by kanghyki          #+#    #+#             */
/*   Updated: 2022/04/26 21:52:32 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/libft.h"

void	ft_putchar_fd(char c, int fd)
{
	size_t	size;
	int		result;

	if (fd < 0)
		return ;
	size = 1;
	while (size > 0)
	{
		result = write(fd, &c, size);
		if (result == -1)
			break ;
		size -= result;
	}
}
