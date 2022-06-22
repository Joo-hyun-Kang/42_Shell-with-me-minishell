#include "cmd.h"

void	ft_execute_pipe(t_argument **arg, int state, t_pipes *pipes)
{
	if (state == INIT)
		state = PIPE_START;
	else if (state == PIPE_START)
		state = PIPE_MIDDLE;
	else if (state == END)
		state = PIPE_END;

	pid_t	pid;
	int		status;

	// if (pipes->current_idx % 2 == 0)
	// 	pipe(pipes->array[pipes->current_idx]);

	pid = fork();
	if (pid == -1)
	{
		printf("minishell : %s\n", strerror(errno));
		//return?
	}
	else if (pid == 0)
	{
		sleep(1);
		ft_set_pipe(pipes, state);
		ft_execute(*arg, false);
	}
	// ls | sort | sort | sort
	//    0

	// if (pipes->current_idx > 1 && pipes->current_idx % 2 == 1)
	// {
		
	// }

	pipes->current_idx++;
	*arg = (*arg)->next;
	if (*arg == NULL)
		return ;

	// 만약에 다음 토큰이 파이프라면 파이프 ㄱㄱ
	// 재귀로 안 짜고 while문으로 빼려면 여기서 빼면 됌
	enum e_token_type token = (*arg)->next_token_type;
	if (token == PIPE)
	{
		ft_execute_pipe(arg, state, pipes);
	}
	else if (token == EOL || token == SEMICOLON)
	{
		ft_execute_pipe(arg, END, pipes);
	}
	else if (ft_is_redir(token))
	{
		ft_execute_redir(arg, state, pipes);
	}


	//free(pipes)
}

int	ft_construt_pipes(t_argument *arg, t_pipes *pipes)
{
	t_argument	*p;
	t_pipes		*temp;
	int			pipe_count;
	int			i;
	int			ret;

	p = arg;
	pipe_count = 0;
	while (p != NULL)
	{
		if (p->next_token_type == PIPE)
		{
			pipe_count++;
		}
		p = p->next;
	}

	pipes->array = (int **)malloc(sizeof(int) * pipe_count);
	pipes->pipe_count = pipe_count;
	pipes->current_idx = 0;
	i = 0;
	while (i < pipe_count)
	{
		pipes->array[i] = (int *)malloc(sizeof(int) * PIPE_COUNT);
		ret = pipe(pipes->array[i]);
		if (ret < -1)
			return (false);
		i++;
	}
	return (true);
}

void	ft_set_pipe(t_pipes *pipes, int state)
{
	int	*cur_pipe;
	int *pre_pipe;
	int	i;

	cur_pipe = pipes->array[pipes->current_idx];
	if (state == PIPE_START)
	{
		//FD1 : STDOUT -> PIPE OUT
		// 파이프 실패 예외처리 해줄것
		dup2(cur_pipe[PIPE_WRITE], STDOUT_FILENO);
		//close(cur_pipe[PIPE_WRITE]);
		//close(cur_pipe[PIPE_READ]);
	}
	else if (state == PIPE_MIDDLE)
	{
		pre_pipe = pipes->array[pipes->current_idx - 1];
		// 파이프 실패 예외처리 해줄것
		dup2(pre_pipe[PIPE_READ], STDIN_FILENO);
		//close(pre_pipe[PIPE_READ]);
		//close(pre_pipe[PIPE_WRITE]);
		
		// 파이프 실패 예외처리 해줄것
		dup2(cur_pipe[PIPE_WRITE], STDOUT_FILENO);
		//close(cur_pipe[PIPE_WRITE]);
		//close(cur_pipe[PIPE_READ]);
	}
	else if (state == PIPE_END)
	{
		pre_pipe = pipes->array[pipes->current_idx - 1];
		// 파이프 실패 예외처리 해줄것
		dup2(pre_pipe[PIPE_READ], STDIN_FILENO);
		//close(pre_pipe[PIPE_READ]);
		//close(pre_pipe[PIPE_WRITE]);
	}

	i = 0;
	while (i < pipes->pipe_count)
	{
		close(pipes->array[i][PIPE_READ]);
		close(pipes->array[i][PIPE_WRITE]);
		i++;
	}
}