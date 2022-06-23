/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jokang <jokang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 02:40:52 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/24 08:52:31 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_execute_exit(t_argument *arg, int is_parent)
{
	unsigned long long	rtn;
	int					is_numeric;
	const int			length = ft_get_length_2d_arr(arg->pa_argument);

	rtn = 0;
	if (length > 1)
	{
		rtn = ft_atoull(arg->pa_argument[1], &is_numeric);
		if (is_numeric != 1)
			ft_error(EXIT_NUMERIC, arg->pa_argument[1], is_parent);
		else if (arg->pa_argument[2] != NULL)
			ft_error(EXIT_NUMERIC, NULL, is_parent);
	}
	if (is_parent != 0)
		printf("exit\n");
	exit(rtn % 256);
}
