/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 00:55:12 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/06 15:55:58 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

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
