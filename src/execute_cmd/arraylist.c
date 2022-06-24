#include "cmd.h"

#define BASIC_CAPACITY (10)

void	init_arraylist(t_arraylist *arraylist)
{
	arraylist->pa_arr = NULL;
	arraylist->type = NULL;
	arraylist->length = 0;
	arraylist->capacity = 0;
}

int	add_arraylist(t_arraylist *arraylist, char *value, int type)
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

int	is_arraylist_full(t_arraylist *arraylist)
{
	if (arraylist->length >= arraylist->capacity)
	{
		return (true);
	}
	return (false);
}

int	allocate_arraylist(t_arraylist *arraylist)
{
	char	**tmp;
	int		*tmp2;

	if (arraylist->pa_arr == NULL || arraylist->capacity == 0)
	{
		arraylist->pa_arr = (char **)malloc(BASIC_CAPACITY * sizeof(char *));
		exit_malloc_fail(arraylist->pa_arr);
		arraylist->type = (int *)malloc(BASIC_CAPACITY * sizeof(int));
		exit_malloc_fail(arraylist->type);
		arraylist->length = 0;
		arraylist->capacity = BASIC_CAPACITY;
		return (true);
	}
	if (arraylist->pa_arr != NULL && arraylist->capacity != 0)
	{
		tmp = (char **)malloc(arraylist->capacity * 2 * sizeof(char *));
		exit_malloc_fail(tmp);
		ft_copy_char_arr_malloc(tmp, arraylist->pa_arr);
		ft_free_list_value(arraylist->pa_arr, arraylist->length);
		tmp = NULL;
		tmp2 = (int *)malloc(arraylist->capacity * 2 * sizeof(int));
		exit_malloc_fail(tmp2);
		ft_memcpy(tmp2, arraylist->type, arraylist->length);
		free(arraylist->type);
		arraylist->type = tmp2;
		arraylist->capacity *= 2;
		return (true);
	}
	return (false);
}
