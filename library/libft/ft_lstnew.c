/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 03:41:34 by kanghyki          #+#    #+#             */
/*   Updated: 2022/02/17 13:15:03 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*data;

	data = (t_list *)malloc(sizeof(t_list));
	if (!data)
		return (0);
	data->content = content;
	data->next = 0;
	return (data);
}
