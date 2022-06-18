#include "cmd.h"

/*
 * 그냥 에러를 출력하고 나가버리는데
 * 후에 조금 더 구체화할 필요성이 있다
 */
void	ft_print_error()
{
	write(1, "Error!", 6);
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

void	ft_fork_execute(t_argument *argument)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		ft_print_error();
	if (pid == 0)
		ft_execute(argument, false);
}

void	ft_execute_single_cmd(t_argument **arg)
{
	enum e_builtin_type		bull_type;

	if (is_builtin((*arg)->pa_argument[COMMAND_POSITION], &bull_type) == true)
		ft_builtin((*arg), bull_type, true);
	else
		ft_fork_execute(*arg);
	(*arg) = (*arg)->next;
}

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

void	ft_execute_mult_cmd(t_argument **arg)
{
	t_pipes				*pa_pipes;
	enum e_token_type	token;
	int	state;
	int	ret;

	
	pa_pipes = (t_pipes *)malloc(sizeof(t_pipes));
	ret = ft_construt_pipes(pa_pipes);
	// 요거 실패했을 떄 처리 따로 할 것
	if (ret == false)
		return ;

	state = INIT;
	while (*arg != NULL)
	{
		token = (*arg)->next_token_type;
		
		if (token == PIPE)
		{
			int temp_pipe;
			ft_execute_pipe(arg, state, pa_pipes);
		}
		else if (ft_is_redir(token))
		{
			state = REDIRECTION;
		}
	}

	close(pa_pipes->pipe1[PIPE_WRITE]);
	close(pa_pipes->pipe1[PIPE_READ]);
	close(pa_pipes->pipe2[PIPE_WRITE]);
	close(pa_pipes->pipe2[PIPE_READ]);

	while (wait(NULL) != -1)
	{
	}
	
	//free(pipes)
}

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
		//ft_execute_redir(arg, state, pipes);
	}
}

void	ft_system(t_argument *argument)
{
	while (argument != NULL)
	{
		// 첫번쨰 친구들은 EOL 이거나 세미콜론은 여기서 막힌다
		if (argument->next_token_type == EOL || argument->next_token_type == SEMICOLON)
			ft_execute_single_cmd(&argument);
		else
			ft_execute_mult_cmd(&argument);
	}
	while (wait(NULL) != -1);
}

int		ft_execute_path(t_argument *arg)
{
	g_exit = execve(arg->pa_argument[COMMAND_POSITION], arg->pa_argument, NULL);
	return 0;
}

int		ft_execute_nopath(t_argument *arg, char *pa_path)
{
	char *pa_orgin_command = ft_strdup(arg->pa_argument[COMMAND_POSITION]);
	free(arg->pa_argument[COMMAND_POSITION]);
	arg->pa_argument[COMMAND_POSITION] = pa_path;

	g_exit = execve(pa_path, arg->pa_argument, NULL);
	return 0;
}

int		ft_execute_except_case(t_argument *arg)
{
	const int	SIZE = 0;
	int			is_command_exist = false;
	char 		*pa_current_path = getcwd(NULL, SIZE);
	char		*pa_path;
	
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(pa_current_path)) != NULL) {
		while ((ent = readdir (dir)) != NULL) 
		{
			if (ft_strcmp(arg->pa_argument[COMMAND_POSITION], ent->d_name) == 0)
			{
				is_command_exist = true;
				break;
			}
		}
	closedir (dir);
	} 
	else 
	{
	/* could not open directory */
		perror ("");
	}

	//get PATH + COMMAND
	pa_path = ft_join_path_command_malloc(pa_current_path, arg->pa_argument[COMMAND_POSITION]);
	free(pa_current_path);
	
	char *pa_orgin_command = ft_strdup(arg->pa_argument[COMMAND_POSITION]);
	free(arg->pa_argument[COMMAND_POSITION]);
	
	//swap arg->pa_argument
	arg->pa_argument[COMMAND_POSITION] = pa_path;

	g_exit = execve(pa_path, arg->pa_argument, NULL);
	return 0;
}

void	ft_execute(t_argument *argument, int is_parent)
{
	int						is_path;
	char					*pa_path;
	char					*command;
	enum e_builtin_type		bull_type;

	//CASE0 : Bulltein 명령어인 경경우	
	command = argument->pa_argument[COMMAND_POSITION];
	bull_type = INVAILD;

	if (is_builtin(command, &bull_type) == true)
	{
		ft_builtin(argument, bull_type, is_parent);
		return ;
	}

	//CASE1 : 절대 경로나 상대경로로 들어오는 경우
	is_path = false;
	if (ft_strchr(argument->pa_argument[COMMAND_POSITION], '/') != false)
		is_path = true;
	else
		is_path = false;

	if (is_path)
	{
		ft_execute_path(argument);
	}

	//CASE2 : 경로 없이 COMMAND만 들어왔는데 환경변수를 훑었을 때 명령어가 있는 경우
	pa_path = ft_search_command_path_malloc(argument->env, argument->pa_argument[COMMAND_POSITION]);
	if (pa_path != NULL)
		ft_execute_nopath(argument, pa_path);
	//CASE3 : 커맨드이고 현재 디렉토리가 커맨드가 있는 곳(일반적으로 환경변수에 등록되는 곳)인 경우
	//	      커맨드와 현재 디렉토리 내에 있으면 그냥 실행함
	if (ft_is_command_dir())
		ft_execute_except_case(argument);
	exit(127);
}

int	ft_is_command_dir()
{
	char		*pa_path;
	const int	SIZE = 0;

	pa_path = getcwd(NULL, SIZE);
	if (ft_strcmp(pa_path, "/usr/local/bin") == 0)
	{
		return (true);
	}
	if (ft_strcmp(pa_path, "/usr/bin") == 0)
	{
		return (true);
	}
	if (ft_strcmp(pa_path, "/bin") == 0)
	{
		return (true);
	}
	if (ft_strcmp(pa_path, "/usr/sbin") == 0)
	{
		return (true);
	}
	if (ft_strcmp(pa_path, "/sbin") == 0)
	{
		return (true);
	}
	if (ft_strcmp(pa_path, "/usr/local/munki") == 0)
	{
		return (true);
	}
	return (false);
}

char	*ft_search_command_path_malloc(t_env_root *root, char *command)
{
	char	*env_path;
	char	**pa_directories;
	int		length;
	int		i;
	int		position;
	char	*pa_temp;
	char	*pa_command_with_path;
	t_env	*env_path_node;

	env_path_node = ft_env_search(root, ft_strdup("PATH"));
	if (env_path_node == NULL)
		env_path = NULL;
	else
		env_path = env_path_node->pa_value;

	if (env_path == NULL)
	{
		//환경변수 PATH가 없는 상태
		return (NULL);
	}

	pa_directories = ft_split(env_path, ':');

	//현재 디렉토리도 넣는 로직 추가
	i = 0;
	position = -1;
	length = ft_strlen(command);
	while (pa_directories[i] != NULL)
	{
		DIR *dir;
		struct dirent *ent;
		if ((dir = opendir(pa_directories[i])) != NULL) {
			while ((ent = readdir (dir)) != NULL) 
			{
				if (ft_strcmp(command, ent->d_name) == 0)
				{
					position = i;
					break;
				}
			}
		closedir (dir);
		} 
		else 
		{
		/* could not open directory */
			perror ("");
			return NULL;
		}
		i++;
		if (position > 0)
		{
			break;
		}
	}
	if (position < 0)
	{
		ft_free_command(pa_directories);
		return (NULL);
	}

	pa_command_with_path = ft_join_path_command_malloc(pa_directories[position], command);
	ft_free_command(pa_directories);
	return (pa_command_with_path);
}

char	*ft_join_path_command_malloc(char *path, char *command)
{
	char	*pa_temp;
	char	*pa_path_with_command;
	
	pa_temp = ft_strjoin(path, "/");
	pa_path_with_command = ft_strjoin(pa_temp, command);
	free(pa_temp);
	return (pa_path_with_command);
}

void	ft_free_command(char **pa_char)
{
	int i;

	i = 0;
	while (pa_char[i] != NULL)
	{
		free(pa_char[i]);
		i++;
	}
	free(pa_char);
}

int	ft_get_length_2d_arr(char **array)
{
	int	i;

	i = 0;
	while (array[i] != NULL)
	{
		i++;
	}
	return (i);
}
