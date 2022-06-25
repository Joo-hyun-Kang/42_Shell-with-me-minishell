/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_set.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jokang <jokang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 16:13:05 by jokang            #+#    #+#             */
/*   Updated: 2022/06/25 16:14:14 by jokang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"

void	ft_split_command(t_redir *redir, t_lst *arg, t_lst **files, char **gt)
{
	char	**strs;
	int		*type;
	int		i;

	*files = (t_lst *)malloc_safe(sizeof(t_lst));
	init_arraylist(*files);
	strs = redir->list_com->pa_arr;
	type = redir->list_com->type;
	i = 0;
	while (i < redir->list_com->length)
	{
		if (type[i] == RE_COM)
			add_arraylist(arg, ft_strdup(strs[i]), NONE);
		else if (type[i] == LT_OPEN)
			add_arraylist(*files, ft_strdup(strs[i]), LT_OPEN);
		else if (type[i] == DLT_OPEN)
			add_arraylist(*files, ft_strdup(strs[i]), DLT_OPEN);
		else if (type[i] == GT_FILE)
			*gt = ft_strdup(strs[i]);
		i++;
	}
}

void	ft_open_non_write(int j, t_lst *files)
{
	int	fd;

	if (files->type[j] == LT_OPEN)
	{
		fd = open(files->pa_arr[j], (O_CREAT | O_TRUNC | O_RDWR), 0644);
		if (fd < 0)
			ft_error(EXE_NO_DIR, files->pa_arr[j], false);
		close(fd);
	}
	else
	{
		fd = open(files->pa_arr[j], (O_CREAT | O_APPEND | O_RDWR), 0644);
		if (fd < 0)
			ft_error(EXE_NO_DIR, files->pa_arr[j], false);
		close(fd);
	}
}

void	ft_open_write(int j, t_lst *files, t_redir *redir)
{
	int	fd;

	if (files->type[j] == LT_OPEN)
	{
		redir->will_stdout_pipe = false;
		fd = open(files->pa_arr[j], (O_CREAT | O_TRUNC | O_RDWR), 0644);
		if (fd < 0)
			ft_error(EXE_NO_DIR, files->pa_arr[j], false);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else
	{
		redir->will_stdout_pipe = false;
		fd = open(files->pa_arr[j], (O_CREAT | O_APPEND | O_RDWR), 0644);
		if (fd < 0)
			ft_error(EXE_NO_DIR, files->pa_arr[j], false);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void	ft_set_opne_file(t_lst *open_files, t_redir *redir)
{
	int	j;

	if (open_files->pa_arr != NULL)
	{
		j = 0;
		while (j < open_files->length - 1)
		{
			ft_open_non_write(j, open_files);
			j++;
		}
		if (j == open_files->length - 1)
		{
			ft_open_write(j, open_files, redir);
		}
	}
}

int	ft_set_redir(t_redir *redir, t_lst *arg)
{
	t_lst		*open_files;
	char		*pa_gt_files;
	char		*lt_dlt_files;
	int			i;

	pa_gt_files = NULL;
	lt_dlt_files = NULL;
	ft_split_command(redir, arg, &open_files, &pa_gt_files);
	if (open_files->pa_arr != NULL)
	{
		lt_dlt_files = open_files->pa_arr[open_files->length - 1];
		if (lt_dlt_files != NULL && pa_gt_files != NULL)
			if (ft_strcmp(lt_dlt_files, pa_gt_files) == 0)
				exit(0);
	}
	ft_make_redir_com(redir, arg);
	add_arraylist(arg, NULL, NONE);
	ft_set_opne_file(open_files, redir);
	ft_set_read_file(pa_gt_files, redir);
	ft_pipe_redir(redir);
	ft_close_pipe(redir->pipes);
	free_arraylist(open_files);
	free(pa_gt_files);
	return (true);
}
