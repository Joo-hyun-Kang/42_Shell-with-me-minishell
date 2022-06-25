#include "cmd.h"

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

	char	**env;
	env = ft_bstenv_to_dpenv(arg->env);
	g_exit = execve(pa_path, arg->pa_argument, env);
	ft_remove_copy_env(env);
	return 0;
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
