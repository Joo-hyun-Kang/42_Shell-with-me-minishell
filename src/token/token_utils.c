/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 00:55:12 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/03 02:56:10 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

bool	ft_is_letter(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_');
}

bool	ft_is_digit(char c)
{
	return ('0' <= c && '9' >= c);
}

int	ft_strlen(char *str)
{
	int	i = 0;
	
	while (str[i])
		++i;
	return (i);
}

char	*ft_strndup(const char *str, size_t n)
{
	size_t	i;
	char	*str_dup;

	i = 0;
	str_dup = (char *)malloc(sizeof(char) * (n + 1));
	if (!str_dup)
		return (0);
	while (str[i] && i < n)
	{
		str_dup[i] = str[i];
		++i;
	}
	str_dup[i] = 0;
	return (str_dup);
}

char	*ft_strchr(const char *str, int c)
{
	while (*str)
	{
		if (*str == (char)c)
			return ((char *)str);
		++str;
	}
	if (!(char)c)
		return ((char *)str);
	return (0);
}
