/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arraylist_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jokang <jokang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 09:44:19 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/24 12:27:52 by jokang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"

void	ft_free_list_value(char **pa_char, int lenght)
{
	int	i;

	i = 0;
	while (i < lenght)
	{
		free(pa_char[i]);
		++i;
	}
	free(pa_char);
}

void	ft_copy_char_arr_malloc(char **dst, char **src)
{
	int	src_len;
	int	i;

	src_len = ft_get_length_2d_arr(src);
	i = 0;
	while (i < src_len)
	{
		*dst = ft_strdup(*src);
		++dst;
		++src;
		++i;
	}
}

void	exit_malloc_fail(void *p)
{
	if (p == NULL)
	{
		exit(1);
	}
}

void	free_arraylist(t_lst *arraylist)
{
	ft_free_list_value(arraylist->pa_arr, arraylist->length);
	free(arraylist->type);
	free(arraylist);
}

int	is_arraylist_full(t_lst *arraylist)
{
	if (arraylist->length >= arraylist->capacity)
	{
		return (true);
	}
	return (false);
}
