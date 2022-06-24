/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 02:42:08 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/24 11:00:04 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_execute_pwd(int is_parent)
{
	char		*pa_path;
	const int	SIZE = 0;

	pa_path = getcwd(NULL, SIZE);
	if (pa_path == NULL)
		ft_error(PWD_ERR, (void *)pa_path, is_parent);
	else
	{
		printf("%s\n", pa_path);
		free(pa_path);
	}
	if (is_parent == 0)
		exit(0);
}
