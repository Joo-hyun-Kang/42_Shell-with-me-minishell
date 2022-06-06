/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 03:40:47 by kanghyki          #+#    #+#             */
/*   Updated: 2022/05/30 19:12:39 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/libft.h"

int	ft_atoi(const char *str)
{
	long long	result;
	int			flag;

	result = 0;
	flag = 1;
	while (*str && ft_strchr(" \n\t\v\r\f", *str))
		++str;
	if (*str && ft_strchr("+-", *str))
	{
		if (*str == '-')
			flag = -1;
		++str;
	}
	while (ft_isdigit(*str))
	{
		result = (result * 10) + (*(str) - '0');
		if (result * flag > 2147483647)
			return (-1);
		if (result * flag < -2147483648)
			return (0);
		++str;
	}
	result *= flag;
	return ((int)(result));
}
