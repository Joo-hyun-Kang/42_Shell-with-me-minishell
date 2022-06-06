/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 03:42:04 by kanghyki          #+#    #+#             */
/*   Updated: 2022/05/30 19:21:43 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/libft.h"

static size_t	ft_count(const char *s, char c)
{
	size_t	cnt;

	cnt = 0;
	while (*s)
	{
		if (*s != c && (*(s + 1) == c || *(s + 1) == 0))
			++cnt;
		++s;
	}
	return (cnt);
}

static int	ft_input(const char *s, char **ret, int size)
{
	*ret = (char *)malloc(sizeof(char) * (size + 1));
	if (!ret)
		return (0);
	ft_strlcpy(*ret, s - size, size + 1);
	return (1);
}

static int	ft_set(const char *s, char **ret, char c)
{
	size_t	i;
	size_t	size;

	i = 0;
	size = 0;
	while (*s)
	{
		if (*s != c)
			++size;
		else if (size > 0)
		{
			if (!ft_input(s, &ret[i++], size))
				return (0);
			size = 0;
		}
		++s;
	}
	if (!*s && size > 0)
		if (!ft_input(s, &ret[i++], size))
			return (0);
	ret[i] = 0;
	return (1);
}

char	**ft_split(const char *s, char c)
{
	char	**ret;

	if (!s)
		return (0);
	ret = (char **)malloc(sizeof(char *) * (ft_count(s, c) + 1));
	if (!ret)
		return (0);
	if (!ft_set(s, ret, c))
	{
		while (*ret)
			free(*ret++);
		free(ret);
		return (0);
	}
	return (ret);
}
