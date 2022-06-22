/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 13:37:33 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/23 02:47:27 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# define BUFFER_SIZE (64)

/* get_next_line.c */
char		*get_next_line(int fd);

/* get_next_line_utils.c */
size_t		gnl_strlen(const char *str);
char		*gnl_find_newline(const char *str);

#endif
