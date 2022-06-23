#include "cmd.h"

void	ft_show_argument_test2(t_argument *arg)
{
	const char	*t_type_str_test[8] = {"ARGUMENT", "PIPE", "SEMICOLON", "LT", "DLT", "GT", "DGT", "EOL"};
	char		**str;

	if (arg == 0)
		return ;
	printf("{ next tokenType: %s }\n", t_type_str_test[arg->next_token_type]);
	str = arg->pa_argument;
	while (*str != 0)
		printf("{ arg->pa_argument: %s }\n", *str++);
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
	
	if ((*arg)->pa_argument[0] == NULL)
		ft_relocate_redir_argument(arg);
	
	t_arraylist *list_arg = (t_arraylist *)malloc(sizeof(t_arraylist));
	t_arraylist *list_com = (t_arraylist *)malloc(sizeof(t_arraylist));
	t_arraylist *argument = (t_arraylist *)malloc(sizeof(t_arraylist));

	// init 만들기
	list_arg->pa_arr = NULL;
	list_com->pa_arr = NULL;
	argument->pa_arr = NULL;

	
	allocate_arraylist(list_arg);
	allocate_arraylist(list_com);
	allocate_arraylist(argument);

	ft_sort_redir_command(arg, list_arg, list_com);

	pid_t pid;
	pid = fork();
	//pid = 0;
	if (pid == -1)
	{
		printf("minishell : %s\n", strerror(errno));
		//return?
	}
	else if (pid == 0)
	{
		if (ft_set_redir(pipes, list_arg, list_com, argument) == true)
		{
			t_argument temp;
			temp.next_token_type = EOL;
			temp.pa_argument = argument->pa_arr;
			temp.env = (*arg)->env;
			temp.next = NULL;
			ft_execute(&temp, false);
		}
		else
		{
			int error_num;
			error_num = 1;
			exit(error_num);
		}
	}

	//free list_arg
	//free list_redir
	//free(argument)
	while (*arg != NULL && ft_is_redir((*arg)->next_token_type) == true)
	{
		*arg = (*arg)->next;
	}

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
		add_arraylist(list_com, ft_strdup(p->pa_argument[i]), RE_COM);
		i++;
	}

	char *gt_file = NULL;
	while (p->next_token_type != PIPE && p->next_token_type != EOL)
	{
		if (p->next_token_type == GT)
		{
			p = p->next;
			int len = ft_get_length_2d_arr(p->pa_argument);
			int i = 0;
			if (len == 1)
			{
				gt_file = p->pa_argument[i]; 
			}
			i++;
			
			while (i < len)
			{
				add_arraylist(list_arg, ft_strdup(p->pa_argument[i]), NONE);
				i++;
			}
		}
		else if (p->next_token_type == LT || p->next_token_type == DLT)
		{
			enum e_token_type token = p->next_token_type;
			p = p->next;
			int len = ft_get_length_2d_arr(p->pa_argument);
			int i = 0;

			if (token == LT)
			{
				add_arraylist(list_com, ft_strdup(p->pa_argument[i]), LT_OPEN);
			}
			else
			{
				add_arraylist(list_com, ft_strdup(p->pa_argument[i]), DLT_OPEN);
			}

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
}

int	ft_set_redir(t_pipes *pipes, t_arraylist *list_arg, t_arraylist *list_com, t_arraylist *argument)
{
	t_arraylist *open_files;
	char		*pa_gt_files;

	open_files = (t_arraylist *)malloc(sizeof(t_arraylist));

	pa_gt_files = NULL;

	// 사용할 변수들을 다시 가져옴
	char **strs = list_com->pa_arr;
	int	*type = list_com->type;
	int i = 0;
	while (i < list_com->length)
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

	// 커맨드를 따서 pa_arguemt를 만듬
	i = 0;
	while (i < list_arg->length)
	{
		add_arraylist(argument, ft_strdup(list_arg->pa_arr[i]), NONE);
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
				fd = open(open_files->pa_arr[j], (O_CREAT | O_TRUNC | O_RDWR), 00644);
				if (fd < 0)
				{
					printf("minishell : %s\n", strerror(errno));
					return (false);
				}
				close(fd);
			}
			else
			{
				fd = open(open_files->pa_arr[j], (O_CREAT | O_APPEND | O_RDWR), 0666);
				if (fd < 0)
				{
					printf("minishell : %s\n", strerror(errno));
					return (false);
				}
				close(fd);
			}
			j++;
		}
		if (j == open_files->length - 1)
		{
			if (open_files->type[j] == LT_OPEN)
			{
				fd = open(open_files->pa_arr[j], (O_CREAT | O_TRUNC | O_RDWR), 0666);
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
			else
			{
				fd = open(open_files->pa_arr[j], (O_CREAT | O_APPEND | O_RDWR), 0666);
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
		}
	}

	
	if (pa_gt_files != NULL)
	{
		int fd;
		fd = open(pa_gt_files, O_RDONLY);
		if (fd < 0)
		{
			printf("minishell : %s\n", strerror(errno));
			return false;
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}


	//free(lt_files)
	//free(dlt_files)
	//free(pa_lt_files)
	//free(pa_gt_files)
	return (true);
}

int	ft_find_next_pipe(t_argument **arg)
{
	t_argument *p;

	p = *arg;
	while (p != NULL)
	{
		if (p->next_token_type == PIPE)
			return (true);
		p = p->next;
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

	// { next tokenType: GT }      ->   { next tokenType: GT } 
	arg1 = *arg;
	// { next tokenType: EOL }     ->   { arg->pa_argument: cat } 
	// { arg->pa_argument: a.txt } ->   { next tokenType: EOL }
	// { arg->pa_argument: cat }   ->   { arg->pa_argument: a.txt }
	arg2 = (*arg)->next;

	arg1->pa_argument = (char **)malloc(sizeof(char *) * (1 + 1));
	arg1->pa_argument[1] = NULL;

	//arg2 cat을 날려버리꺼야
	len = ft_get_length_2d_arr(arg2->pa_argument);
	arg1->pa_argument[0] = ft_strdup(arg2->pa_argument[len - 1]);
	p = (char **)malloc(sizeof(char *) * (len));
	p[len - 1] = NULL;
	
	i = 0;
	while (i < len - 1)
	{
		p[i] = ft_strdup(arg2->pa_argument[i]);
		i++;
	}
	ft_free_command(arg2->pa_argument); // arg2->pa_argumen[0]; free!
	arg2->pa_argument = p;

	// temp = arg1;
	// arg1 = arg2;
	// arg2 = temp;

printf("===============\n");
	ft_show_argument_test2(arg1);
	ft_show_argument_test2(arg2);
}
