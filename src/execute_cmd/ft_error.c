/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jokang <jokang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 00:57:21 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/24 08:25:40 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"

int	ft_cd_error(int error_code, void *free_info)
{
	printf("cd: ");
	if (error_code == CD_NO_DIR)
		printf("%s: No such file or directory\n", (char *)free_info);
	else if (error_code == CD_NO_SET)
		printf("%s not set\n", (char *)free_info);
	free(free_info);
	return (1);
}

int	ft_exit_error(int error_code, void *info)
{
	printf("exit: ");
	if (error_code == EXIT_NUMERIC)
	{
		printf("%s: numeric argument required\n", (char *)info);
		return (255);
	}
	else if (error_code == EXIT_MANY_ARG)
	{
		printf("too many arguments\n");
		return (1);
	}
	return (0);
}

int	ft_pwd_error(int error_code, void *free_info)
{
	printf("pwd: ");
	if (error_code == PWD_ERR)
		printf("can't get current path\n");
	free(free_info);
	return (0);
}

int	ft_unset_error(int error_code, void *info)
{
	printf("unset: ");
	if (error_code == UNSET_INVALID)
		printf("`%s': not a valid identifier\n", (char *)info);
	return (1);
}

int	ft_export_error(int error_code, void *info)
{
	printf("export: ");
	if (error_code == EXPORT_INVALID)
		printf("`%s': not a valid identifier\n", (char *)info);
	return (1);
}

int	ft_exe_error(int error_code, void *info)
{
	printf("%s: ", (char *)info);
	if (error_code == EXE_CMD_NOT)
	{
		printf("command not found\n");
		return (127);
	}
	else if (error_code == EXE_IS_DIR)
	{
		printf("is a directory\n");
		return (126);
	}
	else if (error_code == EXE_NO_DIR)
	{
		printf("No such file or directory\n");
		return (1);
	}
	return (0);
}

void	ft_error(enum e_err_code error_code, void *info, int is_parent)
{
	int	exit_status;

	printf("minishell: ");
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
