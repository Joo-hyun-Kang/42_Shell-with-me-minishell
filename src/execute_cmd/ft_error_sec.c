/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_sec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jokang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 16:29:21 by jokang            #+#    #+#             */
/*   Updated: 2022/06/25 16:29:22 by jokang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"

int	ft_cd_error(int error_code, void *free_info)
{
	ft_putstr_fd("cd: ", 2);
	ft_putstr_fd((char *)free_info, 2);
	if (error_code == CD_NO_DIR)
		ft_putstr_fd(": No such file or directory\n", 2);
	else if (error_code == CD_NO_SET)
		ft_putstr_fd(" not set\n", 2);
	else if (error_code == CD_NOT_DIR)
		ft_putstr_fd(": Not a directory\n", 2);
	free(free_info);
	return (1);
}

int	ft_exit_error(int error_code, void *info)
{
	ft_putstr_fd("exit: ", 2);
	if (error_code == EXIT_NUMERIC)
	{
		ft_putstr_fd((char *)info, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return (255);
	}
	else if (error_code == EXIT_MANY_ARG)
	{
		ft_putstr_fd("too many arguments\n", 2);
		return (1);
	}
	return (0);
}
