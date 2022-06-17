#include "cmd.h"

int	is_builtin(char *command, enum e_builtin_type *out_type)
{
	if (ft_strcmp(command, "echo") == 0)
		*out_type = BUL_ECHO;
	else if (ft_strcmp(command, "cd") == 0)
		*out_type = BUL_CD;
	else if (ft_strcmp(command, "pwd") == 0)
		*out_type = BUL_PWD;
	else if (ft_strcmp(command, "export") == 0)
		*out_type = BUL_EXPORT;
	else if (ft_strcmp(command, "unset") == 0)
		*out_type = BUL_UNSET;
	else if (ft_strcmp(command, "env") == 0)
		*out_type = BUL_ENV;
	else if (ft_strcmp(command, "exit") == 0)
		*out_type = BUL_EXIT;
	else
		*out_type = INVAILD;
	if (*out_type == INVAILD)
		return (false);
	return(true);
}

void    ft_execute_exit(t_argument *argument, int is_parent)
{
	unsigned long long	rtn;
	int					is_numeric;

	if (argument->pa_argument[1] == NULL)
	{
		if (is_parent != 0)
			printf("exit\n");
		exit(0);
	}
	else
	{
		rtn = ft_atoull(argument->pa_argument[1], &is_numeric);
		if (is_numeric != 1)
		{
			printf("bash: exit: %s: numeric argument required\n", argument->pa_argument[1]);
			exit(255);
		}
		else if (argument->pa_argument[2] != NULL)
		{
			printf("bash: exit: too many arguments\n");
			exit(1);
		}	
	}
	exit(rtn % 256);
}

int    ft_try_cd_parent(t_argument *argumnet)
{
	char    *command;


    command = argumnet->pa_argument[COMMAND_POSITION];
	
	if (ft_strcmp(command, "cd") == 0)
		ft_execute_cd(argumnet, true);
	else
		printf("bash: %s: No such file or directory\n", command);
	return 0;
}

void	ft_builtin(t_argument *argument, enum e_builtin_type bull_type, int is_parent)
{
	//내장 명령어 실행
	if (bull_type == BUL_ECHO)
		ft_execute_echo(argument, is_parent);
	else if (bull_type == BUL_CD)
		ft_execute_cd(argument, is_parent);
	else if (bull_type == BUL_PWD)
		ft_execute_pwd(argument, is_parent);
    else if (bull_type == BUL_EXIT)
        ft_execute_exit(argument, is_parent);
	else if (bull_type == BUL_EXPORT )
		ft_execute_export(argument, is_parent);
	else if (bull_type == BUL_ENV )
		ft_execute_env(argument, is_parent);
	else if (bull_type == BUL_UNSET )
		ft_execute_unset(argument, is_parent);
}

void	ft_execute_echo(t_argument *argument, int is_parent)
{
	char	*p;
	int		i;
	int		is_newline;

	is_newline = true;
	i = COMMAND_ARG_POSITION;
	if (argument->pa_argument[i] != NULL && \
	ft_strcmp(argument->pa_argument[COMMAND_ARG_POSITION], "-n") == 0)
	{
		is_newline = false;
		i++;
	}
	while (argument->pa_argument[i] != NULL)
	{
		ft_putstr_fd(argument->pa_argument[i], STDOUT_FILENO);
		ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (is_newline)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
	if (is_parent == 0)
		exit(0);
}

void	ft_execute_pwd(t_argument *argument, int is_parent)
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
	if (is_parent == 0)
		exit(0);
}

void	ft_execute_cd(t_argument *argument, int is_parent)
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
		if (is_parent == 0)
			exit (1);
		return ;
	}

	if (length == 1)
	{
		ft_env_insert(argument->env, ft_strdup("OLDPWD"), getcwd(NULL, 0));
		result = chdir("/");
		ft_env_insert(argument->env, ft_strdup("PWD"), getcwd(NULL, 0));
	}
	else if (length == 2)
	{
		if (ft_strcmp(argument->pa_argument[COMMAND_ARG_POSITION], "-") == 0)
		{
			t_env	*find = ft_env_search(argument->env, ft_strdup("OLDPWD"));
			if (find == NULL)
			{
				printf("OLDPWD Blabla...\n");
			}
			else
			{
				ft_env_insert(argument->env, ft_strdup("OLDPWD"), getcwd(NULL, 0));
				result = chdir(find->pa_value);
			}
		}
		else
		{
			ft_env_insert(argument->env, ft_strdup("OLDPWD"), getcwd(NULL, 0));
			result = chdir(argument->pa_argument[COMMAND_ARG_POSITION]);
		}
		ft_env_insert(argument->env, ft_strdup("PWD"), getcwd(NULL, 0));
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
	if (is_parent == 0)
		exit(1);
}
