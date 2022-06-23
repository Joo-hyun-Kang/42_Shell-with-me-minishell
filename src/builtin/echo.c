/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 02:41:36 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/24 08:52:11 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_execute_echo(t_argument *arg, int is_parent)
{
	int			idx;
	int			nl_flag;
	const int	length = ft_get_length_2d_arr(arg->pa_argument);

	nl_flag = true;
	idx = 1;
	while (arg->pa_argument[idx] != NULL && \
	ft_strcmp(arg->pa_argument[idx], "-n") == 0)
	{
		nl_flag = false;
		++idx;
	}
	while (arg->pa_argument[idx] != NULL)
	{
		if (idx != length - 1)
			printf("%s ", arg->pa_argument[idx]);
		else
			printf("%s", arg->pa_argument[idx]);
		++idx;
	}
	if (nl_flag == true)
		printf("\n");
	if (is_parent == 0)
		exit(0);
}
