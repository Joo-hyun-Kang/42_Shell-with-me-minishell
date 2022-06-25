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