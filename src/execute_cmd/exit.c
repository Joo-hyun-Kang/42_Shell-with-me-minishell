/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jokang <jokang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 02:40:52 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/24 03:10:44 by jokang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"

void	ft_execute_exit(t_argument *argument, int is_parent)
{
	unsigned long long	rtn;
	int					is_numeric;
	const int			length = ft_get_length_2d_arr(argument->pa_argument);

	rtn = 0;
	if (length > 1)
	{
		rtn = ft_atoull(argument->pa_argument[1], &is_numeric);
		if (is_numeric != 1)
			ft_error(EXIT_NUMERIC, argument->pa_argument[1], is_parent);
		else if (argument->pa_argument[2] != NULL)
			ft_error(EXIT_NUMERIC, NULL, is_parent);
	}
	if (is_parent != 0)
		printf("exit\n");
	exit(rtn % 256);
}
