/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_safe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 11:22:07 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/25 11:26:02 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	*malloc_safe(size_t n)
{
	void	*ptr;

	ptr = malloc(n);
	if (ptr == NULL)
	{
		ft_putstr_fd("Fatal: Failed to memory allocate ", 2);
		ft_putnbr_fd(n, 2);
		ft_putstr_fd(" bytes.", 2);
		exit(-1);
	}
	return (ptr);
}
