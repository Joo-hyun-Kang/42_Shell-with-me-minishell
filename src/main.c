/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jokang <jokang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 22:42:17 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/25 17:02:21 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	ft_init_minishell(t_env_root **rnv, char **env, int ac, char **av);

int	main(int argc, char **argv, char **env)
{
	char			*read_line;
	t_env_root		*root_env;
	t_argument		*arg;

	ft_init_minishell(&root_env, env, argc, argv);
	read_line = ft_readline();
	while (read_line != NULL)
	{
		add_history(read_line);
		arg = ft_parser(read_line, root_env);
		if (arg != NULL)
		{
			ft_set_echo();
			ft_system(arg);
			ft_free_argument(arg);
		}
		ft_set_noecho();
		read_line = ft_readline();
	}
	ft_exit();
	return (0);
}

static void	ft_init_minishell(t_env_root **rnv, char **env, int ac, char **av)
{
	(void)ac;
	(void)av;
	signal(SIGINT, ft_sigint);
	signal(SIGQUIT, ft_sigquit);
	ft_set_noecho();
	printf("%s", BANNER);
	*rnv = ft_dpenv_to_bstenv(env);
}
