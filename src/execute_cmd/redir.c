#include "cmd.h"

void	ft_redir_lt(t_argument **arg)
{
	enum e_token_type t_type = (*arg)->next_token_type;

	


	(*arg) = (*arg)->next;
}

void	ft_redir_gt(t_argument **arg)
{
	enum e_token_type	t_type = (*arg)->next_token_type;
	t_argument			*pre_arg;
	int					fd;
	int					fd_temp;

	pre_arg = *arg;
	(*arg) = (*arg)->next;

	int length = ft_get_length_2d_arr((*arg)->pa_argument);
	fd = open((*arg)->pa_argument[length - 1], O_RDONLY);
	//open 함수 예외처리 나중에 더 추가
	if (fd < 0)
	{
		printf("minshell: %s: %s\n", (*arg)->pa_argument[length - 1], strerror(errno));
		return ;
	}

	fd_temp = dup(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);

	ft_execute_single_cmd(&pre_arg);
	close(fd);
	dup2(fd_temp, STDIN_FILENO);
}

void	ft_redir(t_argument **arg)
{
	if ((*arg)->next_token_type == LT
	|| (*arg)->next_token_type == DLT)
		ft_redir_lt(arg);
	else
		ft_redir_gt(arg);
	(*arg) = (*arg)->next;
}

int		ft_is_redir(enum e_token_type token)
{
	if (token == LT)
		return (true);
	else if (token == DLT)
		return (true);
	else if (token == GT)
		return (true);
	else if (token == DGT)
		return (true);
	return (false);
}

// 실행의 단위가 누가 할 건인지에 대한 문제가 있음
	// a.txt < sort -> 이건 안 되지만
	// < a.txt sort -> 이건 된다
void ft_execute_redir(t_argument **arg, int state, t_pipes *pipes)
{
	state = REDIRECTION;
	
	// 이 함수는 여기에 들어온 녀석의 전 상태가 무언인지 간에 리다이렉션 세팅을 해주면 됌
	// 멀티 커맨드가 가능해지는 이유는 뭐임? 결국엔 파이프나 EOL에서 실행하는 거
	// 리다이렉션에서는 연결만 해주면 됌
	
	//token : RD
	//arg : 커맨드 매개변수
	//token : arg
	//arg 커맨드 or 매개변수

	//minishell-4.2$ < a.txt cat
	// { next tokenType: GT }
	// { next tokenType: EOL }
	// { arg->pa_argument: a.txt }
	// { arg->pa_argument: cat }

	enum e_builtin_type token = (*arg)->next_token_type;
	
	// 들어온 명령어와 매개변수를 깔끔하게 만들어준다
	// 예외처리도 하고 
	// ft_sort_redir_argument


	// 리다이렉션 세팅

	*arg = (*arg)->next;
	*arg = (*arg)->next;
	token = (*arg)->next_token_type;

	// 만약에 다음 토큰이 파이프라면 파이프 ㄱㄱ
	if (token == PIPE)
	{
		ft_execute_pipe(arg, state, pipes);
	}
	else if (token == REDIRECTION)
	{
		//리다이렉션
	}
	else if (token == END)
	{
		//실행
	}

}