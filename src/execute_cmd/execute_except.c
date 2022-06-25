/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_except.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jokang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 16:35:46 by jokang            #+#    #+#             */
/*   Updated: 2022/06/25 17:15:47 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_is_command_exist(char *current_path, t_argument *arg)
{
	DIR				*dir;
	struct dirent	*ent;

	dir = opendir(current_path);
	if (dir != NULL)
	{
		ent = readdir (dir);
		while (ent != NULL)
		{
			if (ft_strcmp(arg->pa_argument[COMMAND_POSITION], ent->d_name) == 0)
			{
				return (true);
			}
			ent = readdir (dir);
		}
		closedir (dir);
	}
	else
	{
		perror ("");
	}
	return (false);
}

void	ft_exe_except(char *pa_path, t_argument *arg, char **env)
{
	g_exit = execve(pa_path, arg->pa_argument, env);
	ft_remove_copy_env(env);
}

void	ft_execute_except_case(t_argument *arg)
{
	int			is_command_exist;
	char		*pa_current;
	char		*pa_path;

	is_command_exist = false;
	pa_current = getcwd(NULL, 0);
	is_command_exist = ft_is_command_exist(pa_current, arg);
	if (is_command_exist == false)
		return ;
	pa_path = ft_join_path_command(pa_current, arg->pa_argument[0]);
	free(pa_current);
	free(arg->pa_argument[COMMAND_POSITION]);
	arg->pa_argument[COMMAND_POSITION] = pa_path;
	ft_exe_except(pa_path, arg, ft_bstenv_to_dpenv(arg->env));
}

int	ft_is_command_dir(void)
{
	char		*pa_path;
	const int	size = 0;

	pa_path = getcwd(NULL, size);
	if (ft_strcmp(pa_path, "/usr/local/bin") == 0)
		return (true);
	if (ft_strcmp(pa_path, "/usr/bin") == 0)
		return (true);
	if (ft_strcmp(pa_path, "/bin") == 0)
		return (true);
	if (ft_strcmp(pa_path, "/usr/sbin") == 0)
		return (true);
	if (ft_strcmp(pa_path, "/sbin") == 0)
		return (true);
	if (ft_strcmp(pa_path, "/usr/local/munki") == 0)
		return (true);
	return (false);
}
