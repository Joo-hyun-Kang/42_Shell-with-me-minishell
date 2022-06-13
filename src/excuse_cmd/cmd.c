#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <dirent.h>

#include "../../library/libft/inc/libft.h"
#include "../../include/minishell.h"
#include "cmd.h"

#define COMMAND_POSITION (0)
#define	COMMAND_ARG_POSITION (1)

#define PIPE_COUNT (2)
#define PIPE_ERROR (-1)

#define PIPE_NONE (-1)
#define PIPE_START (0)
#define PIPE_MIDDLE (1)
#define PIPE_END (2)

#define ENV_PATH_NAME_LENGTH (5)

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
	char					*command;
	enum e_bulltein_type	bull_type;
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
		command = argument->pa_argument[COMMAND_POSITION];
		if (is_bulletin(command, &bull_type) == TRUE)
		{
			//내장 명령어 실행
			if (bull_type == BUL_ECHO)
				ft_execute_echo(argument);
			else if (bull_type == BUL_CD && !is_pipe_on)
				ft_execute_cd(argument);
			else if (bull_type == BUL_PWD)
				ft_execute_pwd(argument);
		}
		else
		{
			pid_t child_pid;
			child_pid = fork();

			if (child_pid == -1)
			{
				ft_print_error();
			}
			else if (child_pid == 0 && is_pipe_on == PIPE_NONE)
			{
				ft_execuse(argument->pa_argument);
			}
			else if (child_pid == 0 && is_pipe_on == PIPE_START)
			{
				//FD1 : STDOUT -> PIPE OUT
				// 파이프 실패 예외처리 해줄것
				close(fd_pipe2[PIPE_WRITE]);
				close(fd_pipe2[PIPE_READ]);

				dup2(fd_pipe1[PIPE_WRITE], STDOUT_FILENO);
				close(fd_pipe1[PIPE_WRITE]);
				close(fd_pipe1[PIPE_READ]);

				ft_execuse(argument->pa_argument);
			}
			else if (child_pid == 0 && is_pipe_on == PIPE_MIDDLE)
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
				
				
				ft_execuse(argument->pa_argument);
			}
			else if (child_pid == 0 && is_pipe_on == PIPE_END)
			{
				
				close(fd_pipe1[PIPE_WRITE]);
				close(fd_pipe2[PIPE_WRITE]);

				// 파이프 실패 예외처리 해줄것
				dup2(fd_temp, STDIN_FILENO);
				close(fd_temp);

				close(fd_pipe2[PIPE_READ]);
				close(fd_pipe1[PIPE_READ]);
			
				ft_execuse(argument->pa_argument);
			}
		}
		argument = argument->next;
	}

	close(fd_pipe1[PIPE_READ]);
	close(fd_pipe1[PIPE_WRITE]);
	close(fd_pipe2[PIPE_READ]);
	close(fd_pipe2[PIPE_WRITE]);


	while (wait(NULL) != -1)
	{
	}

	ft_free_argument(pa_orgin_argument);
}

void	ft_execuse(char **pa_argument)
{
	int		is_path;
	char	*pa_path;
	
	//CASE1 : 절대 경로나 상대경로로 들어오는 경우
	if (ft_strchr(pa_argument[COMMAND_POSITION], '/') != FALSE)
		is_path = TRUE;
	else
		is_path = FALSE;

	if (is_path)
	{
		execve(pa_argument[COMMAND_POSITION], pa_argument, NULL);
		printf("bash: %s: No such file or directory\n", pa_argument[COMMAND_POSITION]);
		return ;
	}
	
	//CASE2 : 경로 없이 COMMAND만 들어왔는데 환경변수를 훑었을 때 명령어가 있는 경우
	pa_path = ft_search_command_path_malloc(pa_argument[COMMAND_POSITION]);
	if (pa_path != NULL)
	{
		//swap pa_argument
		char *pa_orgin_command = ft_strdup(pa_argument[COMMAND_POSITION]);
		free(pa_argument[COMMAND_POSITION]);
		pa_argument[COMMAND_POSITION] = pa_path;

		execve(pa_path, pa_argument, NULL);
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
				if (ft_strcmp_temp(pa_argument[COMMAND_POSITION], ent->d_name) == 0)
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
		pa_path = ft_join_path_command_malloc(pa_current_path, pa_argument[COMMAND_POSITION]);
		free(pa_current_path);
		
		char *pa_orgin_command = ft_strdup(pa_argument[COMMAND_POSITION]);
		free(pa_argument[COMMAND_POSITION]);
		
		//swap pa_argument
		pa_argument[COMMAND_POSITION] = pa_path;

		execve(pa_path, pa_argument, NULL);
		printf("bash: %s: No such file or directory\n", pa_orgin_command);
		free(pa_path);
		free(pa_orgin_command);
	}
	printf("bash: %s: command not found\n", pa_argument[COMMAND_POSITION]);
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

void	ft_free_argument(t_argument *pa_argument)
{
	t_argument 	*p;
	t_argument 	*temp;
	int			i;

	p = pa_argument;
	while (p->next != NULL)
	{
		i = 0;
		while (p->pa_argument[i] != NULL)
		{
			free(p->pa_argument[i]);
			i++;
		}
		free(p->pa_argument);
		temp = p->next;
		free(p);
		p = temp;
	}
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

void	ft_execute_echo(t_argument *argument)
{
	char	*p;
	int		i;
	int		is_newline;

	is_newline = TRUE;
	i = COMMAND_ARG_POSITION;
	if (argument->pa_argument[i] != NULL && \
	ft_strncmp(argument->pa_argument[COMMAND_ARG_POSITION], "-n", 2) == 0)
	{
		is_newline = FALSE;
		i++;
	}
	while (argument->pa_argument[i] != NULL)
	{
		ft_putstr_fd(argument->pa_argument[i], STDOUT_FILENO);
		i++;
	}
	if (is_newline)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
}

void	ft_execute_pwd(t_argument *argument)
{
	char		*pa_path;
	int			length;
	const int	SIZE = 0;

	//getcwd 매개변수로 NULL, size가 현재 path보다 작다면
	//그러면 내부에서 path만큼 동적할당 해서 문자열을 반환해준다
	length = ft_get_length_2d_arr(argument->pa_argument);
	if (length > 1)
	{
		ft_putstr_fd("pwd: too many arguments\n", STDOUT_FILENO);
		return ;
	}

	pa_path = getcwd(NULL, SIZE);
	if (pa_path == NULL)
	{
		ft_putstr_fd("pwd: can't get current path\n", STDOUT_FILENO);
	}

	ft_putstr_fd(pa_path, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
}

void	ft_execute_cd(t_argument *argument)
{
	int			result;
	int			length;
	const int	CHDIR_ERROR = -1;
	const int	SECOND_ARG = 1;

	length = ft_get_length_2d_arr(argument->pa_argument);
	//Please check when argument 2 is, some cases is like no error
	if (length > 2)
	{
		ft_putstr_fd("cd: ", STDOUT_FILENO);
		ft_putstr_fd(argument->pa_argument[SECOND_ARG], STDOUT_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDOUT_FILENO);
		return ;
	}

	if (length == 1)
	{
		result = chdir("/");
	}
	else if (length == 2)
	{
		result = chdir(argument->pa_argument[COMMAND_ARG_POSITION]);
	}
	else
	{
		ft_print_error();
	}

	if (result == CHDIR_ERROR)
	{
		ft_putstr_fd("cd: ", STDOUT_FILENO);
		ft_putstr_fd(argument->pa_argument[length - 1], STDOUT_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDOUT_FILENO);
	}
}

// 무지성 문자열 비교
int	is_bulletin(char *command, enum e_bulltein_type *out_type)
{
	if (ft_strncmp(command, "echo", 4) == 0)
		*out_type = BUL_ECHO;
	else if (ft_strncmp(command, "cd", 2) == 0)
		*out_type = BUL_CD;
	else if (ft_strncmp(command, "pwd", 3) == 0)
		*out_type = BUL_PWD;
	else if (ft_strncmp(command, "export", 6) == 0)
		*out_type = BUL_EXPORT;
	else if (ft_strncmp(command, "unset", 5) == 0)
		*out_type = BUL_UNSET;
	else if (ft_strncmp(command, "env", 3) == 0)
		*out_type = BUL_ENV;
	else if (ft_strncmp(command, "exit", 4) == 0)
		*out_type = BUL_EXIT;
	else
		*out_type = INVAILD;
	if (*out_type == INVAILD)
		return (FALSE);
	return(TRUE);
}


int	main(int argc, char **argv, char **environ)
{
	//Test Code
	t_argument *pa_arg;
	t_argument *p;
	
// 	/*
// 	//echo
// 	{
// 		printf("Echo TEST\n");

// 		pa_arg = (t_argument *)malloc(sizeof(t_argument));
// 		p = pa_arg;

// 		printf("my output is \n");

// 		p->next_token_type = SEMICOLON;
// 		p->pa_argument = (char **)malloc(sizeof(char *) * 2 + 1);
// 		p->pa_argument[0] = ft_strdup("echo");
// 		p->pa_argument[1] = ft_strdup("1234");
// 		p->pa_argument[2] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = SEMICOLON;
// 		p->pa_argument = (char **)malloc(sizeof(char *) * 3 + 1);
// 		p->pa_argument[0] = ft_strdup("echo");
// 		p->pa_argument[1] = ft_strdup("-n");
// 		p->pa_argument[2] = ft_strdup("5678");
// 		p->pa_argument[3] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = SEMICOLON;
// 		p->pa_argument = (char **)malloc(sizeof(char *) * 2 + 1);
// 		p->pa_argument[0] = ft_strdup("echo");
// 		p->pa_argument[1] = ft_strdup("-n");
// 		p->pa_argument[2] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = SEMICOLON;
// 		p->pa_argument = (char **)malloc(sizeof(char *) * 1 + 1);
// 		p->pa_argument[0] = ft_strdup("echo");
// 		p->pa_argument[1] = NULL;

// 		ft_system(pa_arg);

// 		printf("\n");
// 	}

// 	//Pwd
// 	{
// 		printf("Pwd TEST\n");

// 		pa_arg = (t_argument *)malloc(sizeof(t_argument));
// 		p = pa_arg;

// 		printf("my output is \n");

// 		p->next_token_type = EOL;
// 		p->pa_argument = (char **)malloc(sizeof(char *) * 1 + 1);
// 		p->pa_argument[0] = ft_strdup("pwd");
// 		p->pa_argument[1] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = EOL;
// 		p->pa_argument = (char **)malloc(sizeof(char *) * 2 + 1);
// 		p->pa_argument[0] = ft_strdup("pwd");
// 		p->pa_argument[1] = ft_strdup("213213");
// 		p->pa_argument[2] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = EOL;
// 		p->pa_argument = (char **)malloc(sizeof(char *) * 3 + 1);
// 		p->pa_argument[0] = ft_strdup("pwd");
// 		p->pa_argument[1] = ft_strdup("213213");
// 		p->pa_argument[2] = ft_strdup("213213");
// 		p->pa_argument[3] = NULL;

// 		p->next = NULL;

// 		ft_system(pa_arg);

// 		printf("\n");
// 	}

// 	//Cd
// 	{
// 		printf("Cd TEST\n");

// 		pa_arg = (t_argument *)malloc(sizeof(t_argument));
// 		p = pa_arg;

// 		printf("my output is \n");

// 		p->next_token_type = EOL;
// 		p->pa_argument = (char **)malloc(sizeof(char *) * 1 + 1);
// 		p->pa_argument[0] = ft_strdup("cd");
// 		p->pa_argument[1] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = EOL;
// 		p->pa_argument = (char **)malloc(sizeof(char *) * 1 + 1);
// 		p->pa_argument[0] = ft_strdup("pwd");
// 		p->pa_argument[1] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = EOL;
// 		p->pa_argument = (char **)malloc(sizeof(char *) * 2 + 1);
// 		p->pa_argument[0] = ft_strdup("cd");
// 		p->pa_argument[1] = ft_strdup("bin");
// 		p->pa_argument[2] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = EOL;
// 		p->pa_argument = (char **)malloc(sizeof(char *) * 1 + 1);
// 		p->pa_argument[0] = ft_strdup("pwd");
// 		p->pa_argument[1] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = EOL;
// 		p->pa_argument = (char **)malloc(sizeof(char *) * 2 + 1);
// 		p->pa_argument[0] = ft_strdup("cd");
// 		p->pa_argument[1] = ft_strdup("~");
// 		p->pa_argument[2] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = EOL;
// 		p->pa_argument = (char **)malloc(sizeof(char *) * 1 + 1);
// 		p->pa_argument[0] = ft_strdup("pwd");
// 		p->pa_argument[1] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = EOL;
// 		p->pa_argument = (char **)malloc(sizeof(char *) * 2 + 1);
// 		p->pa_argument[0] = ft_strdup("cd");
// 		p->pa_argument[1] = ft_strdup("2131313");
// 		p->pa_argument[2] = NULL;
// 		p->next = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = EOL;
// 		p->pa_argument = (char **)malloc(sizeof(char *) * 3 + 1);
// 		p->pa_argument[0] = ft_strdup("cd");
// 		p->pa_argument[1] = ft_strdup("342424");
// 		p->pa_argument[2] = ft_strdup("32131");
// 		p->pa_argument[3] = NULL;

// 		p->next = NULL;

// 		ft_system(pa_arg);

// 		printf("\n");
// 	}

	
// 	//execuse
// 	{
// 		printf("execuse TEST\n");

// 		pa_arg = (t_argument *)malloc(sizeof(t_argument));
// 		p = pa_arg;

// 		printf("my output is \n");

// 		p->next_token_type = EOL;
// 		p->pa_argument = (char **)malloc(sizeof(char *) * (1 + 1));
// 		p->pa_argument[0] = ft_strdup("ls");
// 		p->pa_argument[1] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = EOL;
// 		p->pa_argument = (char **)malloc(sizeof(char *) * (1 + 1 + 1));
// 		p->pa_argument[0] = ft_strdup("cd");
// 		p->pa_argument[1] = ft_strdup("../");
// 		p->pa_argument[2] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = EOL;
// 		p->pa_argument = (char **)malloc(sizeof(char *) * (1 + 1));
// 		p->pa_argument[0] = ft_strdup("pwd");
// 		p->pa_argument[1] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = EOL;
// 		p->pa_argument = (char **)malloc(sizeof(char *) * (1 + 1));
// 		p->pa_argument[0] = ft_strdup("./a.out");
// 		p->pa_argument[1] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = EOL;
// 		p->pa_argument = (char **)malloc(sizeof(char *) * (1 + 1 + 1));
// 		p->pa_argument[0] = ft_strdup("ls");
// 		p->pa_argument[1] = ft_strdup("a");
// 		p->pa_argument[2] = NULL;

// 		p->next = NULL;

// 		ft_system(pa_arg);

// 		printf("END!\n");
// 	}
// 	*/

	
	{
		printf("execuse TEST\n");

		pa_arg = (t_argument *)malloc(sizeof(t_argument));
		p = pa_arg;

		printf("my output is \n");

		p->next_token_type = EOL;
		p->pa_argument = (char **)malloc(sizeof(char *) * (1 + 1));
		p->pa_argument[0] = ft_strdup("../main");
		p->pa_argument[1] = NULL;
		
		p->next = NULL;

		ft_system(pa_arg);

		
		printf("execuse TEST\n");

		pa_arg = (t_argument *)malloc(sizeof(t_argument));
		p = pa_arg;

		printf("my output is \n");

		p->next_token_type = PIPE;
		p->pa_argument = (char **)malloc(sizeof(char *) * (1 + 1));
		p->pa_argument[0] = ft_strdup("sort");
		p->pa_argument[1] = NULL;
		
		p->next = (t_argument *)malloc(sizeof(t_argument));
		p = p->next;
		p->next_token_type = PIPE;
		p->pa_argument = (char **)malloc(sizeof(char *) * (1 + 1));
		p->pa_argument[0] = ft_strdup("ls");
		p->pa_argument[1] = NULL;
		
		p->next = (t_argument *)malloc(sizeof(t_argument));
		p = p->next;
		p->next_token_type = PIPE;
		p->pa_argument = (char **)malloc(sizeof(char *) * (1 + 1));
		p->pa_argument[0] = ft_strdup("sort");
		p->pa_argument[1] = NULL;
		
		p->next = (t_argument *)malloc(sizeof(t_argument));
		p = p->next;
		p->next_token_type = EOL;
		p->pa_argument = (char **)malloc(sizeof(char *) * (1 + 1));
		p->pa_argument[0] = ft_strdup("ls");
		p->pa_argument[1] = NULL;
		p->next = NULL;
		
		ft_system(pa_arg);
		
		printf("END!\n");
	}
}
