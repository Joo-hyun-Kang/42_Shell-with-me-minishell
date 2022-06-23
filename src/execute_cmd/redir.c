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
	t_redir_var	redir;

	redir.will_stdin_pipe = false;
	redir.will_stdout_pipe = false;
	
	// INIT 일 때도 파이프 인지 찾아야 함
	// ls > a.txt | sort -> init인데 파이프 있는 경우
	if (state == INIT)
	{
		redir.will_stdin_pipe = false;
		if (ft_find_next_pipe(arg) == true)		
			redir.will_stdout_pipe = true;
		else
			 redir.will_stdin_pipe = false;
	}
	else if (state == PIPE_START || state == PIPE_MIDDLE)
	{
		redir.will_stdin_pipe = true;
		if (ft_find_next_pipe(arg) == true)
			redir.will_stdout_pipe = true;
		else
			redir.will_stdout_pipe = false;
	}
	
	// 리다이렉션이 이상하게 들어온 경우 재정렬해준다
	if ((*arg)->pa_argument[0] == NULL)
		ft_relocate_redir_argument(arg);
	
	t_arraylist *list_arg = (t_arraylist *)malloc(sizeof(t_arraylist));
	t_arraylist *list_com = (t_arraylist *)malloc(sizeof(t_arraylist));
	t_arraylist *argument = (t_arraylist *)malloc(sizeof(t_arraylist));

	// init 만들기
	init_arraylist(list_arg);
	init_arraylist(list_com);
	init_arraylist(argument);

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
		redir.list_arg = list_arg;
		redir.list_com = list_com;
		redir.pipes = pipes;
		if (ft_set_redir(&redir, argument) == true)
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
	pipes->current_idx++;

	free_arraylist(list_arg);
	free_arraylist(list_com);
	free_arraylist(argument);
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
		else if (state == PIPE_START)
			state = PIPE_MIDDLE;
		if (ft_is_redir((*arg)->next_token_type) == false)
		{
			if ((*arg)->next_token_type == EOL)
				state = PIPE_END;
			ft_execute_pipe(arg, state, pipes);
		}
		else
		{
			ft_execute_redir(arg, state, pipes);
		}
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

int	ft_set_redir(t_redir_var *redir, t_arraylist *argument)
{
	t_arraylist *open_files;
	char		*pa_gt_files;
	char		*lt_dlt_files;
	int			is_same_file;

	open_files = (t_arraylist *)malloc(sizeof(t_arraylist));
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
				{
					printf("minishell : %s\n", strerror(errno));
					return (false);
				}
				close(fd);
			}
			else
			{
				fd = open(open_files->pa_arr[j], (O_CREAT | O_APPEND | O_RDWR), 0744);
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
				redir->will_stdout_pipe = false;
				fd = open(open_files->pa_arr[j], (O_CREAT | O_TRUNC | O_RDWR), 0744);
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
			else
			{
				redir->will_stdout_pipe = false;
				fd = open(open_files->pa_arr[j], (O_CREAT | O_APPEND | O_RDWR), 0744);
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
		{
			printf("minishell : %s\n", strerror(errno));
			return false;
		}
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
	/*
	if (is_same_file == true)
	{
		exit(1);
	}
	*/
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
