/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 02:42:38 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/24 15:46:15 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_get_cdstr(t_argument *arg, int is_parent)
{
	char	*key;
	char	*cdstr;
	t_env	*find;

	cdstr = NULL;
	key = NULL;
	if (ft_get_length_2d_arr(arg->pa_argument) == 1)
		key = "HOME";
	else if (ft_strcmp(arg->pa_argument[1], "-") == 0)
		key = "OLDPWD";
	if (key == NULL)
		cdstr = ft_strdup(arg->pa_argument[1]);
	else
	{
		find = ft_env_search(arg->env, ft_strdup(key));
		if (find != NULL)
			cdstr = ft_strdup(find->pa_value);
		else
			ft_error(CD_NO_SET, key, is_parent);
	}
	return (cdstr);
}

void	ft_execute_cd(t_argument *arg, int is_parent)
{
	char		*cdstr;

	cdstr = ft_get_cdstr(arg, is_parent);
	if (cdstr == NULL)
		return ;
	ft_env_insert(arg->env, ft_strdup("OLDPWD"), getcwd(NULL, 0));
	if (chdir(cdstr) == -1)
	{
		if (errno == 20)
			return (ft_error(CD_NOT_DIR, (void *)cdstr, is_parent));
		return (ft_error(CD_NO_DIR, (void *)cdstr, is_parent));
	}
	free(cdstr);
	ft_env_insert(arg->env, ft_strdup("PWD"), getcwd(NULL, 0));
	if (is_parent == 0)
		exit(g_exit);
	g_exit = 0;
}
