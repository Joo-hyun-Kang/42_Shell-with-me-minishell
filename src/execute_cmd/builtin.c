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
	int		len;

	is_newline = true;
	len = ft_get_length_2d_arr(argument->pa_argument);
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
		// 마지막 매개변수가 아닌 한 ' ' 출력
		if (i != len - 1)
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

void	ft_not_set_err(char *env_key)
{
	printf("minishell: cd: %s not set\n", env_key);
	g_exit = 1;
}

void	ft_chdir_err(char *dir_path)
{
	ft_putstr_fd("minishell: cd: ", STDOUT_FILENO);
	ft_putstr_fd(dir_path, STDOUT_FILENO);
	ft_putstr_fd(": No such file or directory\n", STDOUT_FILENO);
}

void	ft_execute_cd(t_argument *argument, int is_parent)
{
	t_env		*find;
	int			result;
	const int	length = ft_get_length_2d_arr(argument->pa_argument);
	char		*dir_path;
	char		*key;

	key = NULL;
	if (length == 1)
		key = "HOME";
	else if (ft_strcmp(argument->pa_argument[1], "-") == 0)
		key = "OLDPWD";
	if (key == NULL)
		dir_path = argument->pa_argument[1];
	else
	{
		find = ft_env_search(argument->env, ft_strdup(key));
		if (find != NULL)
			dir_path = find->pa_value;
		else
			ft_not_set_err(key);
	}
	ft_env_insert(argument->env, ft_strdup("OLDPWD"), getcwd(NULL, 0));
	if (chdir(dir_path) == -1)
		ft_chdir_err(dir_path);
	ft_env_insert(argument->env, ft_strdup("PWD"), getcwd(NULL, 0));
	if (is_parent == 0)
		exit(1);
}
