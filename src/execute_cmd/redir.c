#include "cmd.h"

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
	
	// INIT 일 때도 파이프 인지 찾아야 함
	// ls > a.txt | sort -> init인데 파이프 있는 경우
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
	t_arraylist *argument = (t_arraylist *)malloc(sizeof(t_arraylist));
	ft_sort_redir_command(arg, list_arg, list_com);

	pid_t pid;
	pid = fork();
	if (pid == -1)
	{
		printf("minishell : %s\n", strerror(errno));
		//return?
	}
	else if (pid == 0)
	{
		ft_set_redir(pipes, list_arg, list_com, argument);
		t_argument temp;
		temp.next_token_type = EOL;
		temp.pa_argument = argument->pa_arr;
		temp.next = NULL;
		ft_execute(&temp, false);
	}

	//free list_arg
	//free list_redir
	//free(argument)

	while (*arg != NULL && ft_is_redir((*arg)->next_token_type) == true)
	{
		*arg = (*arg)->next;
	}
	// EOL SEMICON PIPE 
	// ls < c.txt | sort

	enum e_token_type token = (*arg)->next_token_type;
	if (token == PIPE)
	{
		*arg = (*arg)->next;
		if (state == INIT)
			state = PIPE_START;
		if (state == PIPE_START)
			state = PIPE_MIDDLE;
		ft_execute_pipe(arg, state, pipes);
	}
	else if (token == SEMICOLON)
	{
		return ;
	}
	else if (token == EOL)
	{
		*arg = (*arg)->next;
		return ;
	}
}

void	ft_sort_redir_command(t_argument **arg, t_arraylist *list_arg, t_arraylist *list_com)
{
	t_argument	*p;

	p = *arg;
	int len = ft_get_length_2d_arr(p->pa_argument);
	int i = 0;
	while (i < len)
	{
		add_arraylist(list_arg, ft_strdup(p->pa_argument[i]), RE_COM);
		i++;
	}

	char *gt_file = NULL;
	char *lt_file = NULL;
	while (p->next_token_type != PIPE || p->next_token_type != SEMICOLON || p->next_token_type != EOF)
	{
		if (p->next_token_type == GT)
		{
			p = p->next;
			int len = ft_get_length_2d_arr(p->pa_argument);
			int i = 0;
			if (len == 1)
			{
				gt_file = p->pa_argument[i]; 
				i++;
			}
			
			while (i < len)
			{
				add_arraylist(list_arg, ft_strdup(p->pa_argument[i]), NONE);
				i++;
			}
		}
		//LT, DLT 같이 처리
		else if (p->next_token_type == LT || p->next_token_type == DLT)
		{
			p = p->next;
			int len = ft_get_length_2d_arr(p->pa_argument);
			int i = 0;

			if (p->next_token_type == LT)
			{
				add_arraylist(list_com, ft_strdup(p->pa_argument[i]), LT_OPEN);
			}
			else
			{
				add_arraylist(list_com, ft_strdup(p->pa_argument[i]), DLT_OPEN);
			}

			lt_file = p->pa_argument[i];
			i++;
			while (i < len)
			{
				add_arraylist(list_arg, ft_strdup(p->pa_argument[i]), NONE);
				i++;
			}
		}
		else
		{
			p = p->next;
		}
	}

	if (gt_file != NULL)
	{
		add_arraylist(list_com, ft_strdup(gt_file), GT_FILE);
	}
	if (lt_file != NULL)
	{
		add_arraylist(list_com, ft_strdup(lt_file), LT_FILE);
	}
}

void	ft_set_redir(t_pipes *pipes, t_arraylist *list_arg, t_arraylist *list_com, t_arraylist *argument)
{
	t_arraylist *lt_files;
	t_arraylist *dlt_files;
	char		*pa_lt_files;
	char		*pa_gt_files;

	lt_files = (t_arraylist *)malloc(sizeof(t_arraylist));
	dlt_files = (t_arraylist *)malloc(sizeof(t_arraylist));

	// 사용할 변수들을 다시 가져옴
	char **strs = list_com->pa_arr;
	int	*type = list_com->type;
	int i = 0;
	while (i < list_com->length)
	{
		if (type[i] == RE_COM)
			add_arraylist(argument, ft_strdup(strs[i]), NONE);
		else if (type[i] == LT_OPEN)
			add_arraylist(lt_files, ft_strdup(strs[i]), NONE);
		else if (type[i] == DLT_OPEN)
			add_arraylist(dlt_files, ft_strdup(strs[i]), NONE);
		else if (type[i] == LT_FILE)
			pa_lt_files = ft_strdup(strs[i]);
		else if (type[i] == GT_FILE)
			pa_gt_files = ft_strdup(strs[i]);
		i++;
	}

	// 커맨드를 따서 pa_arguemt를 만듬
	i = 0;
	while (i < list_arg->length)
	{
		add_arraylist(argument, ft_strdup(list_arg->pa_arr[i]), NONE);
		i++;
	}
	add_arraylist(argument, NULL, NONE);

	// 그리고 동시에 파일들을 오픈 시킬 건 오픈 시키고
	if (lt_files->pa_arr != NULL)
	{
		int j = 0;
		int fd;
		while (j < lt_files->length)
		{
			fd = open(lt_files->pa_arr[j], O_CREAT | O_TRUNC, 00644);
			if (fd < 0)
			{
				printf("minishell : %s\n", strerror(errno));
			}
			j++;
		}
	}

	if (dlt_files->pa_arr != NULL)
	{
		int j = 0;
		int fd;
		while (j < dlt_files->length)
		{
			fd = open(dlt_files->pa_arr[j], O_CREAT | O_APPEND, 00644);
			if (fd < 0)
			{
				printf("minishell : %s\n", strerror(errno));
			}
			j++;
		}
	}
	
	// 리다이렉션 시켜서 연결 ㄱㄱ
	// lt인경우 gt인지 lt인지 알아내야 함
	if (pa_lt_files != NULL)
	{
		int fd;
		fd = open(pa_lt_files, O_CREAT | O_TRUNC, 00644);
		dup2(fd, 1);
	}

	if (pa_gt_files != NULL)
	{
		int fd;
		fd = open(pa_gt_files, O_RDONLY);
		dup2(fd, 0);
	}

	//free(lt_files)
	//free(dlt_files)
	//free(pa_lt_files)
	//free(pa_gt_files)
}

int	ft_find_next_pipe(t_argument **arg)
{
	t_argument *p;

	p = *arg;
	while (p != NULL)
	{
		if (p->next_token_type == PIPE)
			return (true);
		p->next;
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