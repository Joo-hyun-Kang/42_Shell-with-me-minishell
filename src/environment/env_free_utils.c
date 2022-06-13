/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_free_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 16:04:09 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/13 00:49:28 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_free_env(char **env)
{
	int	i;

	i = 0;
	while (env[i] != 0)
	{
		free(env[i]);
		++i;
	}
	free(env);
}
