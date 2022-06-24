/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_err.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 09:35:36 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/24 09:38:15 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_system_err(enum e_system_err error_code)
{
	ft_putstr_fd("Warning: ", 2);
	if (error_code == FAILED_MALLOC)
		ft_putstr_fd("malloc() failed, EXIT", 2);
	else if (error_code == FAILED_FORK)
		ft_putstr_fd("fork() failed, EXIT", 2);
	else if (error_code == FAILED_PIPE)
		ft_putstr_fd("pipe() failed, EXIT", 2);
	exit(42);
}
