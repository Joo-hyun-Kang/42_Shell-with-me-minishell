/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arraylist.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jokang <jokang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 10:11:14 by jokang            #+#    #+#             */
/*   Updated: 2022/06/24 20:17:58 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"

#define BASIC_CAPACITY (10)

void	init_arraylist(t_lst *arraylist)
{
	arraylist->pa_arr = NULL;
	arraylist->type = NULL;
	arraylist->length = 0;
	arraylist->capacity = 0;
}

int	add_arraylist(t_lst *arraylist, char *value, int type)
{
	if (!is_arraylist_full(arraylist))
	{
		arraylist->pa_arr[arraylist->length] = value;
		arraylist->type[arraylist->length] = type;
		arraylist->length++;
		return (true);
	}
	else
	{
		allocate_arraylist(arraylist);
		return (add_arraylist(arraylist, value, type));
	}
	return (false);
}

int	allocate_arraylist(t_lst *arraylist)
{
	if (arraylist->pa_arr == NULL || arraylist->capacity == 0)
	{
		allocate_list_first(arraylist);
		return (true);
	}
	if (arraylist->pa_arr != NULL && arraylist->capacity != 0)
	{
		allocate_list_sec(arraylist);
		return (true);
	}
	return (false);
}

void	allocate_list_first(t_lst *arraylist)
{
	arraylist->pa_arr = (char **)malloc(BASIC_CAPACITY * sizeof(char *));
	if (arraylist->pa_arr == NULL)
		ft_system_err(FAILED_MALLOC);
	arraylist->type = (int *)malloc(BASIC_CAPACITY * sizeof(int));
	if (arraylist->type == NULL)
		ft_system_err(FAILED_MALLOC);
	arraylist->length = 0;
	arraylist->capacity = BASIC_CAPACITY;
}

void	allocate_list_sec(t_lst *arraylist)
{
	char	**tmp;
	int		*tmp2;

	tmp = (char **)malloc(arraylist->capacity * 2 * sizeof(char *));
	if (tmp == NULL)
		ft_system_err(FAILED_MALLOC);
	ft_copy_char_arr_malloc(tmp, arraylist->pa_arr);
	ft_free_list_value(arraylist->pa_arr, arraylist->length);
	tmp = NULL;
	tmp2 = (int *)malloc(arraylist->capacity * 2 * sizeof(int));
	if (tmp2 == NULL)
		ft_system_err(FAILED_MALLOC);
	ft_memcpy(tmp2, arraylist->type, arraylist->length);
	free(arraylist->type);
	arraylist->type = tmp2;
	arraylist->capacity *= 2;
}
