#include "cmd.h"

int	ft_set_redir(t_redir *redir, t_lst *argument)
{
	t_lst *open_files;
	char		*pa_gt_files;
	char		*lt_dlt_files;
	int			is_same_file;

	open_files = (t_lst *)malloc(sizeof(t_lst));
	init_arraylist(open_files);
	
	pa_gt_files = NULL;
	lt_dlt_files = NULL;

	// 사용할 변수들을 다시 가져옴
	char **strs = redir->list_com->pa_arr;
	int	*type = redir->list_com->type;
	int i = 0;
	while (i < redir->list_com->length)
	{
		if (type[i] == RE_COM)
			add_arraylist(argument, ft_strdup(strs[i]), NONE);
		else if (type[i] == LT_OPEN)
			add_arraylist(open_files, ft_strdup(strs[i]), LT_OPEN);
		else if (type[i] == DLT_OPEN)
			add_arraylist(open_files, ft_strdup(strs[i]), DLT_OPEN);
		else if (type[i] == GT_FILE)
			pa_gt_files = ft_strdup(strs[i]);
		i++;
	}

	// 같은 파일을 여는 경우 예외처리
	if (open_files->pa_arr != NULL)
	{
		lt_dlt_files = open_files->pa_arr[open_files->length - 1];
		if (lt_dlt_files != NULL && pa_gt_files != NULL)
		{
			if (ft_strcmp(lt_dlt_files, pa_gt_files) == 0)
			{
				exit(0);
			}
		}
	}


	// 커맨드를 따서 pa_arguemt를 만듬
	i = 0;
	while (i < redir->list_arg->length)
	{
		add_arraylist(argument, ft_strdup(redir->list_arg->pa_arr[i]), NONE);
		i++;
	}
	add_arraylist(argument, NULL, NONE);

	// 그리고 동시에 파일들을 오픈 시킬 건 오픈 시키고
	// 가장 마지막 파일들은 빼고
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

	if (pa_gt_files != NULL)
	{
		//printf("3\n");
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

	i = 0;
	while (i < redir->pipes->pipe_count)
	{
		close(redir->pipes->array[i][PIPE_READ]);
		close(redir->pipes->array[i][PIPE_WRITE]);
		i++;
	}

	free_arraylist(open_files);
	free(pa_gt_files);
	return (true);
}