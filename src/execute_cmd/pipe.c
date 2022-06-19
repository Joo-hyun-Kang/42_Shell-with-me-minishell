#include "cmd.h"

void	ft_execute_pipe(t_argument **arg, int state, t_pipes *pipes)
{
	if (state == INIT)
	{
		state = PIPE_START;
		pipes->out_copy = pipes->pipe1[PIPE_READ];
		pipes->out_copy_state = PIPE1_READ;
	}
	else if (state == PIPE_START)
	{
		state = PIPE_MIDDLE;
		if (pipes->out_copy_state == PIPE1_READ)
		{
			pipes->out_copy = pipes->pipe2[PIPE_READ];
			pipes->out_copy_state = PIPE2_READ;
		}
		else
		{
			pipes->out_copy = pipes->pipe1[PIPE_READ];
			pipes->out_copy_state = PIPE1_READ;
		}
	}
	else if (state == PIPE_MIDDLE)
	{
		if (pipes->out_copy_state == PIPE1_READ)
		{
			pipes->out_copy = pipes->pipe2[PIPE_READ];
			pipes->out_copy_state = PIPE2_READ;
		}
		else
		{
			pipes->out_copy = pipes->pipe1[PIPE_READ];
			pipes->out_copy_state = PIPE1_READ;
		}
	}
	else if (state == END)
		state = PIPE_END;

	pid_t pid;
	pid = fork();
	if (pid == -1)
	{
		printf("minishell : %s\n", strerror(errno));
		//return?
	}
	else if (pid == 0)
	{
		ft_set_pipe(pipes, state);
		ft_execute(*arg, false);
	}

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
}

int	ft_construt_pipes(t_pipes *pipes)
{
	int ret1;
	int ret2;

	ret1 = pipe(pipes->pipe1);
	ret2 = pipe(pipes->pipe2);
	pipes->out_copy = pipes->pipe1[PIPE_READ];
	pipes->out_copy_state = PIPE1_READ;

	if (ret1 < -1 || ret2 < -1)
	{
		printf("minishell: %s\n", strerror(errno));
		return (false);
	}
	
	return (true);
}

void	ft_set_pipe(t_pipes *pipes, int state)
{
	if (state == PIPE_START)
	{
		//FD1 : STDOUT -> PIPE OUT
		// 파이프 실패 예외처리 해줄것
		close(pipes->pipe2[PIPE_WRITE]);
		close(pipes->pipe2[PIPE_READ]);

		dup2(pipes->pipe1[PIPE_WRITE], STDOUT_FILENO);
		close(pipes->pipe1[PIPE_WRITE]);
		close(pipes->pipe1[PIPE_READ]);
	}
	else if (state == PIPE_MIDDLE)
	{
		close(pipes->pipe1[PIPE_WRITE]);

		// 파이프 실패 예외처리 해줄것
		dup2(pipes->pipe2[PIPE_WRITE], STDOUT_FILENO);
		close(pipes->pipe2[PIPE_WRITE]);
		close(pipes->pipe2[PIPE_READ]);
		// 파이프 실패 예외처리 해줄것
		dup2(pipes->pipe1[PIPE_READ], STDIN_FILENO);
		close(pipes->pipe1[PIPE_READ]);
		close(pipes->pipe1[PIPE_WRITE]);
	}
	else if (state == PIPE_END)
	{
		close(pipes->pipe1[PIPE_WRITE]);
		close(pipes->pipe2[PIPE_WRITE]);

		// 파이프 실패 예외처리 해줄것
		dup2(pipes->out_copy, STDIN_FILENO);
		close(pipes->out_copy);

		close(pipes->pipe2[PIPE_READ]);
		close(pipes->pipe1[PIPE_READ]);
	}
}