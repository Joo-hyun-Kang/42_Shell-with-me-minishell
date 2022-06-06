#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

#include "../../library/libft/inc/libft.h"
#include "../../include/minishell.h"
#include "cmd.h"

#define COMMAND_POSITION (0)
#define	COMMAND_ARG_POSITION (1)

#define PIPE_COUNT (2)
#define PIPE_ERROR (-1)

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
	int						pipe_fd[PIPE_COUNT];
	int						std_fd[PIPE_COUNT];

	// Create pipe
	if (pipe(pipe_fd) == PIPE_ERROR)
		ft_print_error();


	// Reserve before argument's address for Free 
	pa_orgin_argument = argument;
	

	// loop for execute command
	while (argument != NULL)
	{
		//do pipe setting
		mult_command = argument->next_token_type;
		if (mult_command == PIPE || mult_command == LT || mult_command == DLT || mult_command == GT)
		{
			//FD1 : STDOUT -> PIPE OUT
			std_fd[PIPE_OUT] = dup(STDOUT_FILENO);

			if (pipe_fd[PIPE_OUT] != STDOUT_FILENO)
			{
				dup2(pipe_fd[PIPE_OUT], STDOUT_FILENO);
				close(pipe_fd[PIPE_OUT]);
			}
			else
			{
				ft_print_error();
			}
		}
		else if (mult_command == SEMICOLON || mult_command == DGT || mult_command == EOL)
		{
			//FD0 : STDIN -> PIPE IN
			std_fd[PIPE_IN] = dup(STDIN_FILENO);

			if (pipe_fd[PIPE_IN] != STDIN_FILENO)
			{
				dup2(pipe_fd[PIPE_IN], STDIN_FILENO);
				close(pipe_fd[PIPE_IN]);
			}
			else
			{
				ft_print_error();
			}
			
			//FD1 : PIPE OUT -> STDOUT
			pipe_fd[PIPE_OUT] = dup(STDOUT_FILENO);
			
			if (std_fd[PIPE_OUT] != STDOUT_FILENO)
			{
				dup2(std_fd[PIPE_OUT], STDOUT_FILENO);
				close(std_fd[PIPE_OUT]);
			}
			else
			{
				ft_print_error();
			}
		}
		else
		{
			ft_print_error();
		}
	
		//DGT, 세미콜론이라면 명령을 그냥 다음으로 가면 되고 eof라면 그냥 나가면 된다
		command = argument->pa_argument[COMMAND_POSITION];
		if (is_bulletin(command, &bull_type) == TRUE)
		{
			//내장 명령어 실행
			if (bull_type == BUL_ECHO)
				ft_execute_echo(argument);
		}
		else
		{
			pid_t child_pid;
			child_pid = fork();

			if (child_pid == -1)
			{
				ft_print_error();
			}
			else if (child_pid == 0)
			{
				//자식 프로세스
				char *pa_temp = ft_strjoin("/bin/", command);			
				execve(pa_temp, argument->pa_argument, NULL);
				printf("this is execuse Error");
			}
			else
			{
				//부모 프로세스라면 기다린다
				wait(NULL);
				printf("%s\n", "Hi i'm perent");
			}
			
			// 자식프로세스 제작해서 excuse
			// 만약에 여기서 fork를 한다고 하면
			// 어떻게 될까?

			// 내장 명령어에서 파이프를 쓴다고 하면 자식 프로세스 간의 가
		}
		argument = argument->next;
	}

	//FD0 : FIFE_OUT -> STDIN
	pipe_fd[PIPE_IN] = dup(STDIN_FILENO);

	if (std_fd[PIPE_IN] != STDIN_FILENO)
	{
		dup2(std_fd[PIPE_IN], STDIN_FILENO);
		close(std_fd[PIPE_IN]);
	}
	else
	{
		ft_print_error();
	}

	close(pipe_fd[PIPE_OUT]);
	close(pipe_fd[PIPE_IN]);

	ft_free_argument(pa_orgin_argument);
}

void	ft_free_argument(t_argument *pa_argument)
{
	t_argument *p;
	t_argument *temp;

	p = pa_argument;
	while (p->next != NULL)
	{
		//free에서 pa_argument의 내부에 것 free안해주고 있음
		/*
		i = 0;
		while (p->pa_argument[i] != NULL)
		{
			free(p->pa_argument[i]);
			i++
		}
		*/
		free(p->pa_argument);
		temp = p->next;
		free(p);
		p = temp;
	}
}

void	ft_execute_echo(t_argument *argument)
{
	char	*p;
	int		i;
	int		is_newline;

	is_newline = TRUE;
	i = COMMAND_ARG_POSITION;
	if (ft_strncmp(argument->pa_argument[COMMAND_ARG_POSITION], "-n", 2) == 0)
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
	t_argument *pa_arg;

	pa_arg = (t_argument *)malloc(sizeof(t_argument));
	pa_arg->next_token_type = SEMICOLON;
	pa_arg->pa_argument = (char **)malloc(sizeof(char *) * 2 + 1);
	pa_arg->pa_argument[0] = "echo";
	pa_arg->pa_argument[1] = "1234";
	pa_arg->pa_argument[2] = NULL;
	pa_arg->next = (t_argument *)malloc(sizeof(t_argument));
	
	/*
	t_argument *p = pa_arg->next;
	p->next_token_type = EOL;
	p->pa_argument = (char **)malloc(sizeof(char *) * 2 + 1);
	p->pa_argument[0] = "cat";
	p->pa_argument[1] = "-e";
	p->pa_argument[2] = NULL;
	*/
	pa_arg->next = NULL;

	ft_system(pa_arg);




	// 다음 예제 코드에서 printf가 2번 입력되는 문제가 있다
	/*
	pid_t child_pid = fork();

	char *str[2];

	str[0] = "ps";
	str[1] = NULL;
	if (child_pid == 0)
	{
		printf("HI I'm CHILD0\n");
		execve("/bin/ps", NULL, NULL);
	}
	wait(NULL);
	printf("HI I'm parent");
	*/
}
