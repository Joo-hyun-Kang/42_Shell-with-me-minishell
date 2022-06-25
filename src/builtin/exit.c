/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jokang <jokang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 02:40:52 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/25 15:26:32 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static unsigned long long	ft_atoull(const char *str, int *is_numeric);

void	ft_execute_exit(t_argument *arg, int is_parent)
{
	unsigned long long	rtn;
	int					is_numeric;
	const int			length = ft_get_length_2d_arr(arg->pa_argument);

	if (length > 1)
	{
		rtn = ft_atoull(arg->pa_argument[1], &is_numeric);
		if (is_numeric != 1)
		{
			ft_error(EXIT_NUMERIC, arg->pa_argument[1], is_parent);
			exit(g_exit);
		}
		else if (arg->pa_argument[2] != NULL)
			return (ft_error(EXIT_MANY_ARG, NULL, is_parent));
	}
	if (is_parent != 0)
		printf("exit\n");
	exit(rtn % 256);
}

static unsigned long long	ft_atoull(const char *str, int *is_numeric)
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
