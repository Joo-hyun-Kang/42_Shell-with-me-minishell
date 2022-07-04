/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arraylist.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 10:11:14 by jokang            #+#    #+#             */
/*   Updated: 2022/07/04 20:02:49 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
	arraylist->pa_arr = (char **)malloc_safe(BASIC_CAPACITY * sizeof(char *));
	arraylist->type = (int *)malloc_safe(BASIC_CAPACITY * sizeof(int));
	arraylist->length = 0;
	arraylist->capacity = BASIC_CAPACITY;
}

void	allocate_list_sec(t_lst *arraylist)
{
	char	**tmp;
	int		*tmp2;

	tmp = (char **)malloc_safe(arraylist->capacity * 2 * sizeof(char *));
	ft_copy_char_arr_list(tmp, arraylist->pa_arr, arraylist->length);
	ft_free_list_value(arraylist->pa_arr, arraylist->length);
	arraylist->pa_arr = tmp;
	tmp = NULL;
	tmp2 = (int *)malloc_safe(arraylist->capacity * 2 * sizeof(int));
	ft_intcpy(tmp2, arraylist->type, arraylist->length);
	free(arraylist->type);
	arraylist->type = tmp2;
	arraylist->capacity *= 2;
}
