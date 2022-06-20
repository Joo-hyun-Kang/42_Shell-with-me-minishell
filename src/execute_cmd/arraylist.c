#include "cmd.h"

#define BASIC_CAPACITY (10)

int	add_arraylist(t_arraylist *arraylist, char* value)
{
	if (!is_arraylist_full(arraylist))
	{
		arraylist->pa_arr[arraylist->length] = value;
		arraylist->length++;
		return (true);
	}
	else
	{
		allocate_arraylist(arraylist);
		return (add_arraylist(arraylist, value));
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

	if (arraylist->pa_arr == NULL || arraylist->capacity == 0)
	{
		arraylist->pa_arr = (char **)malloc(BASIC_CAPACITY * sizeof(char *));
		exit_malloc_fail(arraylist->pa_arr);
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
		arraylist->pa_arr = tmp;
		tmp = NULL;
		arraylist->capacity *= 2;
		return (true);
	}
	return (false);
}

void	ft_free_list_value(char **pa_char, int lenght)
{
	int i;

	i = 0;
	while (i < lenght)
	{
		free(pa_char[i]);
		i++;
	}
	free(pa_char);
}

void    ft_copy_char_arr_malloc(char **dst, char **src)
{
	int		src_len;
	int		i;

	src_len = ft_get_length_2d_arr(src);
	
	i = 0;
	while (i < src_len)
	{
		*dst = ft_strdup(*src);
		dst++;
		src++;
		i++;
	}
	return (dst);
}

void	exit_malloc_fail(void *p)
{
	if (p == NULL)
	{
		exit(1);
	}
}

void	free_arraylist(t_arraylist *arraylist)
{
	ft_free_list_value(arraylist->pa_arr);
    free(arraylist);
}