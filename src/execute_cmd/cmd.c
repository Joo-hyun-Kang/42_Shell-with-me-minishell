#include "cmd.h"

/*
 * 그냥 에러를 출력하고 나가버리는데
 * 후에 조금 더 구체화할 필요성이 있다
 */
void	ft_print_error()
{
	write(1, "Error!", 6);
}

void	ft_fork_execute(t_argument *argument)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		ft_print_error();
	if (pid == 0)
		ft_execute(argument, false);
	wait(NULL);
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


void	ft_execute_mult_cmd(t_argument **arg)
{
	t_pipes				*pa_pipes;
	enum e_token_type	token;
	int	state;
	int	ret;
	int			status;

	pa_pipes = (t_pipes *)malloc(sizeof(t_pipes));
	ret = ft_construt_pipes(*arg, pa_pipes);
	// 요거 실패했을 떄 처리 따로 할 것
	if (ret == false)
		return ;

	state = INIT;
	while (*arg != NULL)
	{
		token = (*arg)->next_token_type;
		
		if (token == PIPE)
		{
			ft_execute_pipe(arg, state, pa_pipes);
		}
		else if (ft_is_redir(token))
		{
			ft_execute_redir(arg, state, pa_pipes);
		}
	}

	int i = 0;
	while (i < pa_pipes->pipe_count)
	{
		close(pa_pipes->array[i][PIPE_READ]);
		close(pa_pipes->array[i][PIPE_WRITE]);
		i++;
	}

	while (wait(NULL) != -1)
	{
	}

	// //free(pa_pipes)--> 2차원
}

void	ft_system(t_argument *argument)
{
	while (argument != NULL)
	{
		// 첫번쨰 친구들은 EOL 은 여기서 막힌다
		if (argument->next_token_type == EOL)
			ft_execute_single_cmd(&argument);
		else
			ft_execute_mult_cmd(&argument);
	}
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
	{
		ft_execute_nopath(argument, pa_path);
	}
	//CASE3 : 커맨드이고 현재 디렉토리가 커맨드가 있는 곳(일반적으로 환경변수에 등록되는 곳)인 경우
	//	      커맨드와 현재 디렉토리 내에 있으면 그냥 실행함
	if (ft_is_command_dir())
	{
		ft_execute_except_case(argument);
	}
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
		if ((dir = opendir(pa_directories[i])) != NULL) 
		{
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
