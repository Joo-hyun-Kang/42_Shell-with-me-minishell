#include "cmd.h"

void	ft_split_command(t_redir *redir, t_lst *arg, t_lst **files, char **gt)
{
	char	**strs;
	int		*type;
	int		i;

	*files = (t_lst *)malloc(sizeof(t_lst));
	if (*files == NULL)
		ft_system_err(FAILED_MALLOC);
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

void	ft_set_opne_file(t_lst *open_files, t_redir *redir)
{
	int	j;
	int fd;	
	
	if (open_files->pa_arr != NULL)
	{
		j = 0;
		while (j < open_files->length - 1)
		{
			if (open_files->type[j] == LT_OPEN)
			{
				fd = open(open_files->pa_arr[j], (O_CREAT | O_TRUNC | O_RDWR), 0644);
				if (fd < 0)
					ft_error(EXE_NO_DIR, open_files->pa_arr[j], false);
				close(fd);
			}
			else
			{
				fd = open(open_files->pa_arr[j], (O_CREAT | O_APPEND | O_RDWR), 0644);
				if (fd < 0)
					ft_error(EXE_NO_DIR, open_files->pa_arr[j], false);
				close(fd);
			}
			j++;
		}
		if (j == open_files->length - 1)
		{
			if (open_files->type[j] == LT_OPEN)
			{
				redir->will_stdout_pipe = false;
				fd = open(open_files->pa_arr[j], (O_CREAT | O_TRUNC | O_RDWR), 0644);
				if (fd < 0)
					ft_error(EXE_NO_DIR, open_files->pa_arr[j], false);
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
			else
			{
				redir->will_stdout_pipe = false;
				fd = open(open_files->pa_arr[j], (O_CREAT | O_APPEND | O_RDWR), 0644);
				if (fd < 0)
					ft_error(EXE_NO_DIR, open_files->pa_arr[j], false);
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
		}
	}
}

int	ft_set_redir(t_redir *redir, t_lst *arg)
{
	t_lst		*open_files;
	char		*pa_gt_files;
	char		*lt_dlt_files;
	int			is_same_file;

	pa_gt_files = NULL;
	lt_dlt_files = NULL;
	ft_split_command(redir, arg, &open_files, &pa_gt_files);

	// 같은 파일을 여는 경우 예외처리
	if (open_files->pa_arr != NULL)
	{
		lt_dlt_files = open_files->pa_arr[open_files->length - 1];
		if (lt_dlt_files != NULL && pa_gt_files != NULL)
		{
			if (ft_strcmp(lt_dlt_files, pa_gt_files) == 0)
				exit(0);
		}
	}


	// 커맨드를 따서 pa_arguemt를 만듬
	int i = 0;
	while (i < redir->list_arg->length)
	{
		add_arraylist(arg, ft_strdup(redir->list_arg->pa_arr[i]), NONE);
		++i;
	}
	add_arraylist(arg, NULL, NONE);

	ft_set_opne_file(open_files, redir);

	// 그리고 동시에 파일들을 오픈 시킬 건 오픈 시키고
	// 가장 마지막 파일들은 빼고

	/*
	if (open_files->pa_arr != NULL)
	{
		int j = 0;
		int fd;
		while (j < open_files->length - 1)
		{
			if (open_files->type[j] == LT_OPEN)
			{
				fd = open(open_files->pa_arr[j], (O_CREAT | O_TRUNC | O_RDWR), 0744);
				if (fd < 0)
					ft_error(EXE_NO_DIR, open_files->pa_arr[j], false);
				close(fd);
			}
			else
			{
				fd = open(open_files->pa_arr[j], (O_CREAT | O_APPEND | O_RDWR), 0744);
				if (fd < 0)
					ft_error(EXE_NO_DIR, open_files->pa_arr[j], false);
				close(fd);
			}
			j++;
		}
		if (j == open_files->length - 1)
		{
			if (open_files->type[j] == LT_OPEN)
			{
				redir->will_stdout_pipe = false;
				fd = open(open_files->pa_arr[j], (O_CREAT | O_TRUNC | O_RDWR), 0744);
				if (fd < 0)
					ft_error(EXE_NO_DIR, open_files->pa_arr[j], false);
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
			else
			{
				redir->will_stdout_pipe = false;
				fd = open(open_files->pa_arr[j], (O_CREAT | O_APPEND | O_RDWR), 0744);
				if (fd < 0)
					ft_error(EXE_NO_DIR, open_files->pa_arr[j], false);
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
		}
	}
	*/

	if (pa_gt_files != NULL)
	{
		int fd;
		fd = open(pa_gt_files, O_RDONLY);
		if (fd < 0)
			ft_error(EXE_NO_DIR, pa_gt_files, false);
		dup2(fd, STDIN_FILENO);
		close(fd);
		redir->will_stdin_pipe = false;
	}

	if (redir->will_stdin_pipe == true || redir->will_stdout_pipe == true)
	{
		if (redir->will_stdin_pipe == true)
		{
			dup2(redir->pipes->array[redir->pipes->current_idx - 1][PIPE_READ], STDIN_FILENO);
		}
		if (redir->will_stdout_pipe == true)
		{
			dup2(redir->pipes->array[redir->pipes->current_idx][PIPE_WRITE], STDOUT_FILENO);
		}
	}


	ft_close_pipe(redir->pipes);
	free_arraylist(open_files);
	free(pa_gt_files);
	return (true);
}
