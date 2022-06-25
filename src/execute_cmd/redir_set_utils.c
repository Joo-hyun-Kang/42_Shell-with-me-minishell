#include "cmd.h"

void	ft_set_read_file(char *pa_gt_files, t_redir *redir)
{
	int fd;
	
	if (pa_gt_files != NULL)
	{
		fd = open(pa_gt_files, O_RDONLY);
		if (fd < 0)
			ft_error(EXE_NO_DIR, pa_gt_files, false);
		dup2(fd, STDIN_FILENO);
		close(fd);
		redir->will_stdin_pipe = false;
	}
}

void	ft_pipe_redir(t_redir *redir)
{
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
}