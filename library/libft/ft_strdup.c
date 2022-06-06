/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 03:42:10 by kanghyki          #+#    #+#             */
/*   Updated: 2022/05/30 19:18:25 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/libft.h"

char	*ft_strdup(const char *s)
{
	size_t	i;
	char	*s_dup;

	i = 0;
	s_dup = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!s_dup)
		return (0);
	while (s[i])
	{
		s_dup[i] = s[i];
		++i;
	}
	s_dup[i] = 0;
	return (s_dup);
}
