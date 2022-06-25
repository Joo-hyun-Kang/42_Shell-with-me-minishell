/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jokang <jokang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 00:57:21 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/25 16:24:34 by jokang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"

int	ft_pwd_error(int error_code, void *free_info)
{
	ft_putstr_fd("pwd: ", 2);
	if (error_code == PWD_ERR)
		ft_putstr_fd("can't get current path\n", 2);
	free(free_info);
	return (0);
}

int	ft_unset_error(int error_code, void *info)
{
	ft_putstr_fd("unset: ", 2);
	if (error_code == UNSET_INVALID)
	{
		ft_putchar_fd('`', 2);
		ft_putstr_fd((char *)info, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
	}
	return (1);
}

int	ft_export_error(int error_code, void *info)
{
	ft_putstr_fd("export: ", 2);
	if (error_code == EXPORT_INVALID)
	{
		ft_putchar_fd('`', 2);
		ft_putstr_fd((char *)info, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
	}
	return (1);
}

int	ft_exe_error(int error_code, void *info)
{
	ft_putstr_fd((char *)info, 2);
	ft_putstr_fd(": ", 2);
	if (error_code == EXE_CMD_NOT)
	{
		ft_putstr_fd("command not found\n", 2);
		return (127);
	}
	else if (error_code == EXE_NO_DIR)
	{
		ft_putstr_fd("No such file or directory\n", 2);
		return (127);
	}
	else if (error_code == EXE_PERMISSION)
	{
		ft_putstr_fd("Permission denied\n", 2);
		return (126);
	}

	return (0);
}

void	ft_error(enum e_err_code error_code, void *info, int is_parent)
{
	int	exit_status;

	ft_putstr_fd("minishell: ", 2);
	if (error_code >= CD_MIN && error_code <= CD_MAX)
		exit_status = ft_cd_error(error_code, info);
	else if (error_code >= PWD_MIN && error_code <= PWD_MAX)
		exit_status = ft_pwd_error(error_code, info);
	else if (error_code >= EXIT_MIN && error_code <= EXIT_MAX)
		exit_status = ft_exit_error(error_code, info);
	else if (error_code >= UNSET_MIN && error_code <= UNSET_MAX)
		exit_status = ft_unset_error(error_code, info);
	else if (error_code >= EXPORT_MIN && error_code <= EXPORT_MAX)
		exit_status = ft_export_error(error_code, info);
	else if (error_code >= EXE_MIN && error_code <= EXE_MAX)
		exit_status = ft_exe_error(error_code, info);
	if (is_parent == 0)
		exit (exit_status);
	g_exit = exit_status;
}
