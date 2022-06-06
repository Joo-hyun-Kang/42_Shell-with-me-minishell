/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 03:41:36 by kanghyki          #+#    #+#             */
/*   Updated: 2022/05/30 19:15:36 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/libft.h"

int	ft_lstsize(t_list *lst)
{
	size_t	lst_size;

	lst_size = 0;
	while (lst)
	{
		lst = lst -> next;
		++lst_size;
	}
	return (lst_size);
}
