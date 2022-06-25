#include "cmd.h"

int		ft_execute_nopath(t_argument *arg, char *pa_path)
{
	char	**env;
	char	*pa_orgin_command;

	pa_orgin_command = ft_strdup(arg->pa_argument[COMMAND_POSITION]);
	free(arg->pa_argument[COMMAND_POSITION]);
	arg->pa_argument[COMMAND_POSITION] = pa_path;

	env = ft_bstenv_to_dpenv(arg->env);
	g_exit = execve(pa_path, arg->pa_argument, env);
	ft_remove_copy_env(env);
	return 0;
}

void	ft_find_dir_pos(char *command, char **pa_directories, int *position)
{
	struct dirent	*ent;
	DIR				*dir;
	int 			i;

	i = 0;
	while (pa_directories[i] != NULL)
	{
		if ((dir = opendir(pa_directories[i])) != NULL) 
		{
			while ((ent = readdir (dir)) != NULL) 
			{
				if (ft_strcmp(command, ent->d_name) == 0)
				{
					*position = i;
					break;
				}
			}
			closedir (dir);
		} 
		else 
			return ;
		i++;
		if (*position > 0)
			break;
	}
}

char	*ft_search_command_path_malloc(t_env_root *root, char *command)
{
	char	*env_path;
	char	**pa_directories;
	int		position;
	char	*pa_command_with_path;
	t_env	*env_path_node;

	env_path_node = ft_env_search(root, ft_strdup("PATH"));
	if (env_path_node == NULL)
		env_path = NULL;
	else
		env_path = env_path_node->pa_value;
	if (env_path == NULL)
		return (NULL);
	pa_directories = ft_split(env_path, ':');
	position = -1;
	ft_find_dir_pos(command, pa_directories, &position);
	if (position < 0)
	{
		ft_free_command(pa_directories);
		return (NULL);
	}
	pa_command_with_path = ft_join_path_command_malloc(pa_directories[position], command);
	ft_free_command(pa_directories);
	return (pa_command_with_path);
}