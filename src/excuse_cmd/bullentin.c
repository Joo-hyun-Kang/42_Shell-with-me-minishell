#include "cmd.h"

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

void	ft_bulletin(t_argument *argument, enum e_bulltein_type bull_type)
{
	//내장 명령어 실행
	if (bull_type == BUL_ECHO)
		ft_execute_echo(argument);
	else if (bull_type == BUL_CD)
		ft_execute_cd(argument);
	else if (bull_type == BUL_PWD)
		ft_execute_pwd(argument);
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