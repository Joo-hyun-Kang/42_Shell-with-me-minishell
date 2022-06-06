/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 03:41:14 by kanghyki          #+#    #+#             */
/*   Updated: 2022/05/30 19:13:35 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/libft.h"

static size_t	ft_get_size(long long num)
{
	size_t	size;

	size = 1;
	if (num < 0)
	{
		++size;
		num *= -1;
	}
	while (num >= 10)
	{
		num /= 10;
		++size;
	}
	return (size);
}

static char	*ft_input_num(char *str, long long num, size_t size)
{
	if (num < 0)
	{
		str[0] = '-';
		num *= -1;
	}
	while (num >= 10)
	{
		str[size--] = (num % 10) + '0';
		num /= 10;
	}
	str[size] = (num % 10) + '0';
	return (str);
}

char	*ft_itoa(int n)
{
	long long	num;
	size_t		size;
	char		*str;

	num = n;
	size = ft_get_size(num);
	str = (char *)malloc(sizeof(char) * (size + 1));
	if (!str)
		return (0);
	str[size] = 0;
	ft_input_num(str, num, size - 1);
	return (str);
}
