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

void	ft_system(t_argument *argument)
{
	t_argument				*pa_orgin_argument;
	enum e_token_type		mult_command;
	int						fd_pipe1[PIPE_COUNT];
	int						fd_pipe2[PIPE_COUNT];
	int						fd_current_pipe[PIPE_COUNT];
	int						fd_temp;
	int						is_pipe_on;

	// Reserve before argument's address for Free
	pa_orgin_argument = argument;

	// loop for execute command
	
	is_pipe_on = PIPE_NONE;
	while (argument != NULL)
	{
		mult_command = argument->next_token_type;
		
		if (mult_command == PIPE && is_pipe_on == PIPE_NONE)
		{
			is_pipe_on = PIPE_START;
			pipe(fd_pipe1);
			pipe(fd_pipe2);
			fd_temp = fd_pipe1[PIPE_READ];
		}
		else if (mult_command == PIPE && is_pipe_on == PIPE_START)
		{
			is_pipe_on = PIPE_MIDDLE;
			//pipe(fd_pipe2);
			fd_temp = fd_pipe2[PIPE_READ];

		}
		else if (mult_command != PIPE && (is_pipe_on == PIPE_START || is_pipe_on == PIPE_MIDDLE))
		{
			is_pipe_on = PIPE_END;
		}

		//DGT, 세미콜론이라면 명령을 그냥 다음으로 가면 되고 eof라면 그냥 나가면 된다

		//자식 쉘 생성 전에 부모 쉘 종료시키는 경우
		if (ft_try_exit_parent(argument) == FALSE)
		{
			argument = argument->next;
			continue;
		}
	
		pid_t child_pid;
		child_pid = fork();
		
		//자식 쉘을 생성한다
		if (child_pid == -1)
		{
			ft_print_error();
		}
		if (child_pid == 0)
		{
			if (is_pipe_on == PIPE_NONE)
			{
				ft_execuse(argument);
			}
			else if (is_pipe_on == PIPE_START)
			{
				//FD1 : STDOUT -> PIPE OUT
				// 파이프 실패 예외처리 해줄것
				close(fd_pipe2[PIPE_WRITE]);
				close(fd_pipe2[PIPE_READ]);

				dup2(fd_pipe1[PIPE_WRITE], STDOUT_FILENO);
				close(fd_pipe1[PIPE_WRITE]);
				close(fd_pipe1[PIPE_READ]);

				ft_execuse(argument);
			}
			else if (is_pipe_on == PIPE_MIDDLE)
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
					
				ft_execuse(argument);
			}
			else if (is_pipe_on == PIPE_END)
			{
				
				close(fd_pipe1[PIPE_WRITE]);
				close(fd_pipe2[PIPE_WRITE]);

				// 파이프 실패 예외처리 해줄것
				dup2(fd_temp, STDIN_FILENO);
				close(fd_temp);

				close(fd_pipe2[PIPE_READ]);
				close(fd_pipe1[PIPE_READ]);
			
				ft_execuse(argument);
			}
		}
		argument = argument->next;
	}

	if (is_pipe_on == PIPE_END)
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

void	ft_execuse(t_argument *argument)
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
		ft_bulletin(argument, bull_type);
		return ;
	}

	//CASE1 : 절대 경로나 상대경로로 들어오는 경우
	if (ft_strchr(argument->pa_argument[COMMAND_POSITION], '/') != FALSE)
		is_path = TRUE;
	else
		is_path = FALSE;

	if (is_path)
	{
		execve(argument->pa_argument[COMMAND_POSITION], argument->pa_argument, NULL);
		printf("bash: %s: No such file or directory\n", argument->pa_argument[COMMAND_POSITION]);
		return ;
	}
	
	//CASE2 : 경로 없이 COMMAND만 들어왔는데 환경변수를 훑었을 때 명령어가 있는 경우
	pa_path = ft_search_command_path_malloc(argument->pa_argument[COMMAND_POSITION]);
	if (pa_path != NULL)
	{
		//swap argument->pa_argument
		char *pa_orgin_command = ft_strdup(argument->pa_argument[COMMAND_POSITION]);
		free(argument->pa_argument[COMMAND_POSITION]);
		argument->pa_argument[COMMAND_POSITION] = pa_path;

		execve(pa_path, argument->pa_argument, NULL);
		printf("bash: %s: No such file or directory\n", pa_orgin_command);
		free(pa_path);
		free(pa_orgin_command);
		return ;
	}

	//CASE3 : 커맨드이고 현재 디렉토리가 커맨드가 있는 곳(일반적으로 환경변수에 등록되는 곳)인 경우
	//	      커맨드와 현재 디렉토리 내에 있으면 그냥 실행함
	if (ft_is_command_dir())
	{
		const int	SIZE = 0;
		int	is_command_exist = FALSE;
		char *pa_current_path = getcwd(NULL, SIZE);
		
		DIR *dir;
		struct dirent *ent;
		if ((dir = opendir(pa_current_path)) != NULL) {
			while ((ent = readdir (dir)) != NULL) 
			{
				if (ft_strcmp_temp(argument->pa_argument[COMMAND_POSITION], ent->d_name) == 0)
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
		pa_path = ft_join_path_command_malloc(pa_current_path, argument->pa_argument[COMMAND_POSITION]);
		free(pa_current_path);
		
		char *pa_orgin_command = ft_strdup(argument->pa_argument[COMMAND_POSITION]);
		free(argument->pa_argument[COMMAND_POSITION]);
		
		//swap argument->pa_argument
		argument->pa_argument[COMMAND_POSITION] = pa_path;

		execve(pa_path, argument->pa_argument, NULL);
		printf("bash: %s: No such file or directory\n", pa_orgin_command);
		free(pa_path);
		free(pa_orgin_command);
	}
	printf("bash: %s: command not found\n", argument->pa_argument[COMMAND_POSITION]);
}

int	ft_is_command_dir()
{
	char		*pa_path;
	const int	SIZE = 0;

	pa_path = getcwd(NULL, SIZE);
	if (ft_strcmp_temp(pa_path, "/usr/local/bin") == 0)
	{
		return (TRUE);
	}
	if (ft_strcmp_temp(pa_path, "/usr/bin") == 0)
	{
		return (TRUE);
	}
	if (ft_strcmp_temp(pa_path, "/bin") == 0)
	{
		return (TRUE);
	}
	if (ft_strcmp_temp(pa_path, "/usr/sbin") == 0)
	{
		return (TRUE);
	}
	if (ft_strcmp_temp(pa_path, "/sbin") == 0)
	{
		return (TRUE);
	}
	if (ft_strcmp_temp(pa_path, "/usr/local/munki") == 0)
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
				if (ft_strcmp_temp(command, ent->d_name) == 0)
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

int	ft_strcmp_temp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] != 0 && s2[i] != 0)
	{
		if (s1[i] != s2[i])
			break ;
		++i;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

// void	ft_free_argument(t_argument *pa_argument)
// {
// 	t_argument 	*p;
// 	t_argument 	*temp;
// 	int			i;

// 	p = pa_argument;
// 	while (p->next != NULL)
// 	{
// 		i = 0;
// 		while (p->pa_argument[i] != NULL)
// 		{
// 			free(p->pa_argument[i]);
// 			i++;
// 		}
// 		free(p->pa_argument);
// 		temp = p->next;
// 		free(p);
// 		p = temp;
// 	}
// }

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

void	ft_get_sh_command(char **src_2d, char **out_dst)
{
	int	i;
	int j;
	int	length;

	i = 0;
	out_dst[i++] = ft_strdup("/bin/sh");
	out_dst[i++] = ft_strdup("-c");

	j = 0;
	while (src_2d[j] != NULL)
	{
		out_dst[i] = ft_strdup(src_2d[j]);
		i++;
		j++;
	}
	out_dst[i] = NULL;
}
