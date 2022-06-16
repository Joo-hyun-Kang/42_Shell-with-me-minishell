/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_merge_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 04:12:34 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/16 15:22:16 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*ft_dupfree(char *str);

char	*ft_merge_str(char *s1, char *s2)
{
	size_t	s1_size;
	size_t	s2_size;
	char	*str;

	if (!s1 && !s2)
		return (NULL);
	else if (s1 == NULL)
		return (ft_dupfree(s2));
	else if (s2 == NULL)
		return (ft_dupfree(s1));
	s1_size = ft_strlen(s1);
	s2_size = ft_strlen(s2);
	str = (char *)malloc(sizeof(char) * (s1_size + s2_size + 1));
	if (str == NULL)
		return (NULL);
	ft_strlcpy(str, s1, s1_size + 1);
	ft_strlcat(str, s2, s1_size + s2_size + 1);
	free(s1);
	free(s2);
	return (str);
}

static char	*ft_dupfree(char *str)
{
	char	*dup;

	dup = ft_strdup(str);
	free(str);
	return (dup);
}
