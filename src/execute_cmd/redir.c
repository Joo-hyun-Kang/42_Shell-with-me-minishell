#include "cmd.h"


	// ls > a.txt
	// 	{ next tokenType: LT }
	// { arg->pa_argument: ls }
	// { next tokenType: EOL }
	// { arg->pa_argument: a.txt }


void	ft_redir_lt(t_argument **arg)
{
	
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

	// ls > a.txt
	// 	{ next tokenType: LT }
	// { arg->pa_argument: ls }
	// { next tokenType: EOL }
	// { arg->pa_argument: a.txt }


	
	// 들어온 명령어와 매개변수를 깔끔하게 만들어준다
	if (arg->pa_argument == NULL)
		ft_relocate_redir_argument(arg, command);

	enum e_builtin_type token = (*arg)->next_token_type;

	// 리다이렉션 세팅
	if (token == LT)
		ft_redir_lt(argument);
	else if (token == DLT)
		return 
	else if (token == GT)
		return (true);
	else if (token == DGT)
		return (true);
	return (false);


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

void ft_relocate_redir_argumet(t_argument **arg)
{
	t_argument	*arg1;
	t_argument	*arg2;
	t_argument	*temp;
	char		**p;
	int			len;
	int			i;
	
	//"< a.txt cat"를 "cat < a.txt" 토큰 형태로 바꾼다
	// { next tokenType: GT }      ->   { next tokenType: GT } 
	// { next tokenType: EOL }     ->   { arg->pa_argument: cat } 
	// { arg->pa_argument: a.txt } ->   { next tokenType: EOL }
	// { arg->pa_argument: cat }   ->   { arg->pa_argument: a.txt }
	arg1 = *arg;
	arg2 = (*arg)->next;

	arg1->pa_argument = (char **)malloc(sizeof(char *) * (1 + 1));
	arg1->pa_argument[0] = ft_strdup(arg2->pa_argument[0]);
	arg1->pa_argument[1] = NULL;

	len = ft_get_length_2d_arr(arg2->pa_argument);
	p = (char **)malloc(sizeof(char *) * (len));
	
	i = 0;
	while (i < len)
	{
		p[i] = ft_strdup(arg2->pa_argument[i + 1]);
		i++;
	}
	free(arg2->pa_argument);
	arg2->pa_argumet = p;

	temp = arg1;
	arg1 = arg2;
	arg2 = temp;
}