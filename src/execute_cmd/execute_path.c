/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jokang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 16:31:09 by jokang            #+#    #+#             */
/*   Updated: 2022/06/25 17:15:49 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_execute_nopath(t_argument *arg, char *pa_path)
{
	char	**env;

	free(arg->pa_argument[COMMAND_POSITION]);
	arg->pa_argument[COMMAND_POSITION] = pa_path;
	env = ft_bstenv_to_dpenv(arg->env);
	g_exit = execve(pa_path, arg->pa_argument, env);
	ft_remove_copy_env(env);
	return (0);
}

void	ft_find_dir_pos(char *command, char **pa_directories, int *p)
{
	struct dirent	*ent;
	DIR				*dir;
	int				i;

	i = 0;
	while (pa_directories[i] != NULL)
	{
		dir = opendir(pa_directories[i]);
		if (dir != NULL)
		{
			ent = readdir(dir);
			while (ent != NULL)
			{
				if (ft_strcmp(command, ent->d_name) == 0)
					*p = i;
			}
			closedir(dir);
		}
		else
			return ;
		i++;
		if (*p > 0)
			break ;
	}
}

char	*ft_search_command_path(t_env_root *root, char *command)
{
	char	*env_path;
	char	**pa_directories;
	int		position;
	char	*command_with_path;
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
	command_with_path = ft_join_path_command(pa_directories[position], command);
	ft_free_command(pa_directories);
	return (command_with_path);
}
