/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 03:42:36 by kanghyki          #+#    #+#             */
/*   Updated: 2022/05/30 19:19:19 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strrchr(const char *str, int c)
{
	int	i;

	i = 0;
	while (str[i])
		++i;
	while (i >= 0)
	{
		if (str[i] == (char)c)
			return ((char *)&str[i]);
		--i;
	}
	return (0);
}
