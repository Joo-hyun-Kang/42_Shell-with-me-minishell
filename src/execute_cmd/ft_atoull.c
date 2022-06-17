/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoull.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 04:27:42 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/18 04:28:04 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"

unsigned long long	ft_atoull(const char *str, int *is_numeric)
{
	unsigned long long	result;
	int					minus;

	result = 0;
	minus = 1;
	*is_numeric = 1;
	while (*str && ft_strchr(" \n\t\v\r\f", *str))
		++str;
	if (*str && ft_strchr("+-", *str))
	{
		if (*str == '-')
			minus = -1;
		++str;
	}
	while (ft_isdigit(*str))
	{
		result = (result * 10) + (*(str) - '0');
		if (minus == 1 && result > 9223372036854775808ULL)
			*is_numeric = 0;
		if (minus == 0 && result > 9223372036854775807ULL)
			*is_numeric = 0;
		++str;
	}
	if (*str != '\0')
		*is_numeric = -1;
	return ((unsigned long long)(result *= minus));
}
