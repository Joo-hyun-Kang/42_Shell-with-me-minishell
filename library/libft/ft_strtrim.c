/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 03:42:38 by kanghyki          #+#    #+#             */
/*   Updated: 2022/05/30 19:19:31 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	size_t	front;
	size_t	rear;

	if (!s1 || !set)
		return (0);
	front = 0;
	rear = ft_strlen(s1);
	while (s1[front] && ft_strchr(set, s1[front]))
		++front;
	while (s1[rear - 1] && rear > front && ft_strchr(set, s1[rear - 1]))
		--rear;
	str = (char *)malloc(sizeof(char) * (rear - front + 1));
	if (!str)
		return (0);
	ft_strlcpy(str, s1 + front, rear - front + 1);
	return (str);
}
