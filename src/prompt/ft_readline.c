/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 19:29:45 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/24 19:31:16 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*ft_replace_cur_dir(char *pa_path);

char	*ft_readline(void)
{
	char	*pa_path;
	char	*read_line;

	pa_path = getcwd(NULL, 0);
	if (pa_path == NULL)
		pa_path = ft_strdup("Undefined");
	else
		pa_path = ft_replace_cur_dir(pa_path);
	pa_path = ft_merge_str(ft_strdup(" \033[1;33m"), pa_path);
	pa_path = ft_merge_str(pa_path, ft_strdup("\033[0;0m "));
	pa_path = ft_merge_str(strdup(STT_READ), pa_path);
	pa_path = ft_merge_str(pa_path, strdup(END_READ));
	read_line = readline(pa_path);
	free(pa_path);
	return (read_line);
}

static char	*ft_replace_cur_dir(char *pa_path)
{
	char	*pos;
	char	*cur_dir;

	pos = ft_strrchr(pa_path, '/');
	if (ft_strlen(pos) > 1)
		cur_dir = ft_strdup(pos + 1);
	else
		cur_dir = ft_strdup("/");
	free(pa_path);
	return (cur_dir);
}
