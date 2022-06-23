/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 03:37:32 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/23 03:37:39 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_exit(void)
{
	printf("\033[1A\033[15Cexit\n");
	exit(g_exit);
}
