/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 03:40:58 by kanghyki          #+#    #+#             */
/*   Updated: 2022/02/17 13:14:02 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/libft.h"

void	*ft_calloc(size_t n, size_t size)
{
	void	*init;

	init = (void *)malloc(sizeof(void) * (size * n));
	if (!init)
		return (0);
	ft_bzero(init, size * n);
	return (init);
}
