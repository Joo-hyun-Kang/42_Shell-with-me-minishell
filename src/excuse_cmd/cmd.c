#include "../../include/minishell.h"
#include "cmd.h"

// 테스팅용 전역 환경변수

extern char **environ;

/*
 * 그냥 에러를 출력하고 나가버리는데
 * 후에 조금 더 구체화할 필요성이 있다
 */
void	ft_print_error()
{
	write(1, "Error!", 6);
}

int	ft_parent(t_argument *arg)
{
	if(ft_strcmp(arg->pa_argument[COMMAND_POSITION], "exit") == 0)
	{
		ft_execute_exit(arg, TRUE);
		return (1);
	}
	else if(ft_strcmp(arg->pa_argument[COMMAND_POSITION], "cd") == 0)
	{
		ft_try_cd_parent(arg);
		return (1);
	}
		return (0);
}

void	ft_state_redirection(t_argument **arg, int fd_pipe1[2], int fd_pipe2[2], int fd_temp)
{
	int	fd;
	char *asd;

	*arg = (*arg)->next;
	asd = ft_strjoin(getcwd(NULL, 0), ft_strjoin("/", (*arg)->pa_argument[0]));

	printf("%s\n", asd);	
	fd = open(asd, O_RDONLY);
	if (fd == -1)
	{
		printf("그런파일없다\n");
	}
	else
	{
		printf("리다이렉션 구현할게\n");
	}
	close(fd);
	exit(1);
}

void	ft_state_pipe(int fd_pipe1[2], int fd_pipe2[2], int fd_temp, int pipe_state)
{
	if (pipe_state == PIPE_START)
	{
		//FD1 : STDOUT -> PIPE OUT
		// 파이프 실패 예외처리 해줄것
		close(fd_pipe2[PIPE_WRITE]);
		close(fd_pipe2[PIPE_READ]);

		dup2(fd_pipe1[PIPE_WRITE], STDOUT_FILENO);
		close(fd_pipe1[PIPE_WRITE]);
		close(fd_pipe1[PIPE_READ]);
	}
	else if (pipe_state == PIPE_MIDDLE)
	{
		close(fd_pipe1[PIPE_WRITE]);

		// 파이프 실패 예외처리 해줄것
		dup2(fd_pipe2[PIPE_WRITE], STDOUT_FILENO);
		close(fd_pipe2[PIPE_WRITE]);
		close(fd_pipe2[PIPE_READ]);
		// 파이프 실패 예외처리 해줄것
		dup2(fd_pipe1[PIPE_READ], STDIN_FILENO);
		close(fd_pipe1[PIPE_READ]);
		close(fd_pipe1[PIPE_WRITE]);
	}
	else if (pipe_state == PIPE_END)
	{
		close(fd_pipe1[PIPE_WRITE]);
		close(fd_pipe2[PIPE_WRITE]);

		// 파이프 실패 예외처리 해줄것
		dup2(fd_temp, STDIN_FILENO);
		close(fd_temp);

		close(fd_pipe2[PIPE_READ]);
		close(fd_pipe1[PIPE_READ]);
	}
}

void	ft_system(t_argument *argument)
{
	t_argument				*pa_orgin_argument;
	enum e_token_type		mult_command;
	int						fd_pipe1[PIPE_COUNT];
	int						fd_pipe2[PIPE_COUNT];
	int						fd_current_pipe[PIPE_COUNT];
	int						fd_temp;
	int						pipe_state;

	// Reserve before argument's address for Free
	pa_orgin_argument = argument;

	// loop for execute command
	
	if (argument->next == NULL)
		if (ft_parent(argument) == 1)
			return ;

	pipe_state = PIPE_NONE;
	while (argument != NULL)
	{
		mult_command = argument->next_token_type;
		
		if (mult_command == EOL || mult_command == SEMICOLON)
			pipe_state = PIPE_NONE;
		else if (mult_command == PIPE)
		{
			if (pipe_state == PIPE_NONE)
			{
				pipe_state = PIPE_START;
				pipe(fd_pipe1);
				pipe(fd_pipe2);
				fd_temp = fd_pipe1[PIPE_READ];
			}
			else if (pipe_state == PIPE_START)
			{
				pipe_state = PIPE_MIDDLE;
				//pipe(fd_pipe2);
				fd_temp = fd_pipe2[PIPE_READ];

			}
			else if (pipe_state == PIPE_START || pipe_state == PIPE_MIDDLE)
			{
				pipe_state = PIPE_END;
			}
		}
		else
			pipe_state = REDIRECTION;

		pid_t child_pid;
		child_pid = fork();

		if (child_pid == -1)
			ft_print_error();
		if (child_pid == 0)
		{
			if (pipe_state == REDIRECTION)
				ft_state_redirection(&argument, fd_pipe1, fd_pipe2, fd_temp);
			else
			{
				ft_state_pipe(fd_pipe1, fd_pipe2, fd_temp, pipe_state);
				ft_execuse(argument, child_pid);
			}
		}
		argument = argument->next;
	}

	if (pipe_state == PIPE_END)
	{
		close(fd_pipe1[PIPE_READ]);
		close(fd_pipe1[PIPE_WRITE]);
		close(fd_pipe2[PIPE_READ]);
		close(fd_pipe2[PIPE_WRITE]);
	}

	while (wait(NULL) != -1)
	{
	}

	ft_free_argument(pa_orgin_argument);
}

int		ft_execuse_path(t_argument *arg)
{
	g_exit = execve(arg->pa_argument[COMMAND_POSITION], arg->pa_argument, NULL);
	return 0;
}

int		ft_execuse_nopath(t_argument *arg, char *pa_path)
{
	char *pa_orgin_command = ft_strdup(arg->pa_argument[COMMAND_POSITION]);
	free(arg->pa_argument[COMMAND_POSITION]);
	arg->pa_argument[COMMAND_POSITION] = pa_path;

	g_exit = execve(pa_path, arg->pa_argument, NULL);
	return 0;
}

int		ft_execuse_except_case(t_argument *arg)
{
	const int	SIZE = 0;
	int			is_command_exist = FALSE;
	char 		*pa_current_path = getcwd(NULL, SIZE);
	char		*pa_path;
	
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(pa_current_path)) != NULL) {
		while ((ent = readdir (dir)) != NULL) 
		{
			if (ft_strcmp(arg->pa_argument[COMMAND_POSITION], ent->d_name) == 0)
			{
				is_command_exist = TRUE;
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

void	ft_execuse(t_argument *argument, int is_parent)
{
	int						is_path;
	char					*pa_path;
	char					*command;
	enum e_bulltein_type	bull_type;

	//CASE0 : Bulltein 명령어인 경경우	
	command = argument->pa_argument[COMMAND_POSITION];
	bull_type = INVAILD;

	if (is_bulletin(command, &bull_type))
	{
		ft_bulletin(argument, bull_type, is_parent);
		return ;
	}

	//CASE1 : 절대 경로나 상대경로로 들어오는 경우
	if (ft_strchr(argument->pa_argument[COMMAND_POSITION], '/') != FALSE)
		is_path = TRUE;
	else
		is_path = FALSE;

	if (is_path)
		ft_execuse_path(argument);

	//CASE2 : 경로 없이 COMMAND만 들어왔는데 환경변수를 훑었을 때 명령어가 있는 경우
	pa_path = ft_search_command_path_malloc(argument->pa_argument[COMMAND_POSITION]);
	if (pa_path != NULL)
		ft_execuse_nopath(argument, pa_path);

	//CASE3 : 커맨드이고 현재 디렉토리가 커맨드가 있는 곳(일반적으로 환경변수에 등록되는 곳)인 경우
	//	      커맨드와 현재 디렉토리 내에 있으면 그냥 실행함
	if (ft_is_command_dir())
		ft_execuse_except_case(argument);
	exit(127);
}

int	ft_is_command_dir()
{
	char		*pa_path;
	const int	SIZE = 0;

	pa_path = getcwd(NULL, SIZE);
	if (ft_strcmp(pa_path, "/usr/local/bin") == 0)
	{
		return (TRUE);
	}
	if (ft_strcmp(pa_path, "/usr/bin") == 0)
	{
		return (TRUE);
	}
	if (ft_strcmp(pa_path, "/bin") == 0)
	{
		return (TRUE);
	}
	if (ft_strcmp(pa_path, "/usr/sbin") == 0)
	{
		return (TRUE);
	}
	if (ft_strcmp(pa_path, "/sbin") == 0)
	{
		return (TRUE);
	}
	if (ft_strcmp(pa_path, "/usr/local/munki") == 0)
	{
		return (TRUE);
	}
	return (FALSE);
}

char	*ft_search_command_path_malloc(char *command)
{
	char	**pp;
	char	*env_path;
	char	**pa_directories;
	int		length;
	int		i;
	int		position;
	char	*pa_temp;
	char	*pa_command_with_path;

	env_path = NULL;
	pp = environ;
	while (*pp != NULL)
	{
		if (ft_strncmp(*pp, "PATH", 4) == 0)
		{
			env_path = *pp;
			break;
		}
		pp++;
	}

	if (env_path == NULL)
	{
		//환경변수 PATH가 없는 상태
		return (NULL);
	}

	if (env_path != NULL)
	{
		int env_path_length = ft_strlen(env_path);
		if (env_path_length > ENV_PATH_NAME_LENGTH)
		{
			env_path += ENV_PATH_NAME_LENGTH;
		}
		else
		{
			//환경 변수 'PATH='인 상태 
			return (NULL);
		}
		pa_directories = ft_split(env_path, ':');
	}

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