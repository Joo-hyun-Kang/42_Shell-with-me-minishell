/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 13:37:10 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/23 02:51:30 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/get_next_line.h"

static char	*gnl_strndup(const char *s, size_t n);
static char	*gnl_strjoin(char *s1, char *s2);
static char	*gnl_split_by_newline(char **storage);
static char	*gnl_endline(char **storage, int rs);

static char	*gnl_strndup(const char *s, size_t n)
{
	size_t	i;
	char	*s_dup;

	i = 0;
	if (gnl_strlen(s) < n)
		n = gnl_strlen(s);
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

static char	*gnl_strjoin(char *s1, char *s2)
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
			return (gnl_strndup(s2, gnl_strlen(s2)));
		else
			return (gnl_strndup(s1, gnl_strlen(s1)));
	}
	str = (char *)malloc(sizeof(char) * (gnl_strlen(s1) + gnl_strlen(s2) + 1));
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

static char	*gnl_split_by_newline(char **storage)
{
	char	*front;
	char	*rear;
	char	*loc_newline;

	loc_newline = gnl_find_newline(*storage);
	front = gnl_strndup(*storage, loc_newline - *storage + 1);
	rear = gnl_strndup(loc_newline + 1, gnl_strlen(loc_newline + 1));
	free(*storage);
	*storage = rear;
	return (front);
}

static char	*gnl_endline(char	**storage, int rs)
{
	char	*rtn;

	rtn = 0;
	if (*storage != NULL)
	{
		if (gnl_strlen(*storage) > 0 && !(rs < 0))
			rtn = *storage;
		else
			free(*storage);
		*storage = 0;
	}
	return (rtn);
}

char	*get_next_line(int fd)
{
	static char		*storage = 0;
	char			buf[BUFFER_SIZE + 1];
	int				rs;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (0);
	if (gnl_find_newline(storage))
		return (gnl_split_by_newline(&storage));
	rs = read(fd, buf, BUFFER_SIZE);
	while (rs > 0)
	{
		buf[rs] = 0;
		storage = gnl_strjoin(storage, buf);
		if (gnl_find_newline(storage))
			return (gnl_split_by_newline(&storage));
		rs = read(fd, buf, BUFFER_SIZE);
	}
	return (gnl_endline(&storage, rs));
}
