#include "cmd.h"

void	ft_redir_lt(t_argument **arg)
{
	t_argument	*pre_arg;
	int			file_count;

	pre_arg = *arg;
	(*arg) = (*arg)->next;

	file_count = ft_get_length_2d_arr((*arg)->pa_argument);
	if (file_count > 1)
	{
		
	}
	

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

void ft_execute_redir(t_argument **arg, int state, t_pipes *pipes)
{
	int will_stdin_pipe;
	int	will_stdout_pipe;
	
	if (state == INIT)
	{
		will_stdin_pipe = false;
		will_stdout_pipe = false;
	}
	else if (state == PIPE_START || state == PIPE_MIDDLE)
	{
		will_stdin_pipe = true;
		if (ft_find_next_pipe(arg) == true)
			will_stdout_pipe = true;
		else
			will_stdout_pipe = false;
	}
	
	// 리다이렉션이 이상하게 들어온 경우 재정렬해준다
	if ((*arg)->pa_argument == NULL)
		ft_relocate_redir_argument(arg);

	t_arraylist *list_arg = (t_arraylist *)malloc(sizeof(t_arraylist));
	t_arraylist *list_com = (t_arraylist *)malloc(sizeof(t_arraylist));
	ft_sort_redir_command(arg, list_arg, list_redir);

	*arg = (*arg)->next;
	*arg = (*arg)->next;
	if (*arg == NULL)
		return ;
	token = (*arg)->next_token_type;


	//free list_arg
	//free list_redir

	// 만약에 다음 토큰이 파이프라면 파이프 ㄱㄱ
	// 재귀로 안 짜고 while문으로 빼려면 여기서 빼면 됌
	if (token == PIPE)
	{
		if (state == INIT)
			state = PIPE_START;
		if (state == PIPE_START)
			state = PIPE_MIDDLE);
		ft_execute_pipe(arg, state, pipes);
	}
}

void	ft_sort_redir_command(t_argument **arg, t_arraylist *list_arg, t_arraylist *list_com)
{
	t_argument	*p;

	//COMM 복사
	p = *arg;
	add_arraylist(list_com, ft_strdup("COMMAD"));
	int len = ft_get_length_2d_arr(p->pa_argument);
	int i = 0;
	while (i < len)
	{
		add_arraylist(list_arg, ft_strdup(p->pa_argument[i]));
		i++;
	}

	while (p->next_token_type != PIPE || p->next_token_type != SEMICOLON || p->next_token_type != EOF)
	{
		if (p->next_token_type == GT)
		{
			p = p->next;
			int len = ft_get_length_2d_arr(p->pa_argument);
			int i = 0;
			if (len == 1)
			{
				
				i++;
			}
			
			while (i < len)
			{
				add_arraylist(list_arg, ft_strdup(p->pa_argument[i]));
				i++;
			}
		}
		else if (p->next_token_type == LT)
		{
			p = p->next;
			int len = ft_get_length_2d_arr(p->pa_argument);
			int i = 0;

			add_arraylist(list_com, ft_strdup("LT"));
			add_arraylist(list_com, ft_strdup(p->pa_argument[i]));
			i++;
			while (i < len)
			{
				add_arraylist(list_arg, ft_strdup(p->pa_argument[i]));
				i++;
			}
		}
	
	}
	
}


int	ft_find_next_pipe(t_argument **arg)
{
	t_argument *p;

	p = *arg;
	while (*p != NULL)
	{
		if (p->next_token_type == PIPE)
			return (true);
	}
	return (false);
}

void ft_relocate_redir_argument(t_argument **arg)
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
	arg2->pa_argument = p;

	temp = arg1;
	arg1 = arg2;
	arg2 = temp;
}