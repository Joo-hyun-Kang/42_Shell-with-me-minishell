/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_safe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 15:09:12 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/25 15:16:17 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	pipe_safe(int pp[2])
{
	int	rtn;

	rtn = pipe(pp);
	if (rtn != 0)
	{
		ft_putstr_fd("Fatal: Failed to pipe()", 2);
		exit(-1);
	}
	return (rtn);
}
