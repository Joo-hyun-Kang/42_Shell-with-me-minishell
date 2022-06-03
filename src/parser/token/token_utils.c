/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 00:55:12 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/04 02:13:20 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

char	*ft_strjoin(char *s1, char *s2)
{
	int		index;
	char	*str;
	char	*loc_s1;

	index = 0;
	loc_s1 = s1;
	if (!s1 && !s2)
		return (0);
	else if (!s1 || !s2)
	{
		if (!s1)
			return (ft_strndup(s2, ft_strlen(s2)));
		else
			return (ft_strndup(s1, ft_strlen(s1)));
	}
	str = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!str)
		return (0);
	while (*s1)
		str[index++] = *(s1++);
	while (*s2)
		str[index++] = *(s2++);
	str[index] = 0;
	free(loc_s1);
	return (str);
}

int	ft_strlen(const char *str)
{
	int	i = 0;
	
	while (str[i])
		++i;
	return (i);
}

char	*ft_strndup(const char *s, int n)
{
	int		i;
	char	*s_dup;

	i = 0;
	if (ft_strlen(s) < n)
		n = ft_strlen(s);
	s_dup = (char *)malloc(sizeof(char) * (n + 1));
	if (!s_dup)
		return (0);
	while (i < n)
	{
		s_dup[i] = s[i];
		++i;
	}
	s_dup[i] = 0;
	return (s_dup);
}

char	*ft_strchr(const char *str, int c)
{
	while (*str)
	{
		if (*str == (char)c)
			return ((char *)str);
		++str;
	}
	return (0);
}
