/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 02:41:36 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/24 02:41:42 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"

void	ft_execute_echo(t_argument *argument, int is_parent)
{
	int			idx;
	int			nl_flag;
	const int	length = ft_get_length_2d_arr(argument->pa_argument);

	nl_flag = true;
	idx = 1;
	while (argument->pa_argument[idx] != NULL && \
	ft_strcmp(argument->pa_argument[idx], "-n") == 0)
	{
		nl_flag = false;
		++idx;
	}
	while (argument->pa_argument[idx] != NULL)
	{
		if (idx != length - 1)
			printf("%s ", argument->pa_argument[idx]);
		else
			printf("%s", argument->pa_argument[idx]);
		++idx;
	}
	if (nl_flag == true)
		printf("\n");
	if (is_parent == 0)
		exit(0);
}
