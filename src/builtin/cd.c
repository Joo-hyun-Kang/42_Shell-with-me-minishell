/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 02:42:38 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/24 11:01:59 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_execute_cd(t_argument *arg, int is_parent)
{
	t_env		*find;
	int			result;
	char		*dir_path;
	char		*key;

	key = NULL;
	if (ft_get_length_2d_arr(arg->pa_argument) == 1)
		key = ft_strdup("HOME");
	else if (ft_strcmp(arg->pa_argument[1], "-") == 0)
		key = ft_strdup("OLDPWD");
	if (key == NULL)
		dir_path = ft_strdup(arg->pa_argument[1]);
	else
	{
		find = ft_env_search(arg->env, ft_strdup(key));
		if (find != NULL)
			dir_path = ft_strdup(find->pa_value);
		else
			return (ft_error(CD_NO_SET, key, is_parent));
	}
	ft_env_insert(arg->env, ft_strdup("OLDPWD"), getcwd(NULL, 0));
	if (chdir(dir_path) == -1)
		return (ft_error(CD_NO_DIR, (void *)dir_path, is_parent));
	free(dir_path);
	ft_env_insert(arg->env, ft_strdup("PWD"), getcwd(NULL, 0));
	if (is_parent == 0)
		exit(g_exit);
	g_exit = 0;
}
