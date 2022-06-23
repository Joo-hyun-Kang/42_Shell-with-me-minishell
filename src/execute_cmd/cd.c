/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 02:42:38 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/24 02:42:44 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"

void	ft_execute_cd(t_argument *argument, int is_parent)
{
	t_env		*find;
	int			result;
	char		*dir_path;
	char		*key;

	key = NULL;
	if (ft_get_length_2d_arr(argument->pa_argument) == 1)
		key = ft_strdup("HOME");
	else if (ft_strcmp(argument->pa_argument[1], "-") == 0)
		key = ft_strdup("OLDPWD");
	if (key == NULL)
		dir_path = ft_strdup(argument->pa_argument[1]);
	else
	{
		find = ft_env_search(argument->env, ft_strdup(key));
		if (find != NULL)
			dir_path = ft_strdup(find->pa_value);
		else
			return (ft_error(CD_NO_SET, key, is_parent));
	}
	ft_env_insert(argument->env, ft_strdup("OLDPWD"), getcwd(NULL, 0));
	if (chdir(dir_path) == -1)
		return (ft_error(CD_NO_DIR, (void *)dir_path, is_parent));
	ft_env_insert(argument->env, ft_strdup("PWD"), getcwd(NULL, 0));
	if (is_parent == 0)
		exit(g_exit);
	else
		g_exit = 0;
}
