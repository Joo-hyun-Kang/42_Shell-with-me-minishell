/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jokang <jokang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 22:42:17 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/24 19:45:54 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	ft_init_minishell(t_env_root **rnv, char **env, int ac, char **av);
static void	ft_show_argument_test(t_argument *arg); // for test

int main(int argc, char **argv, char **env)
{
	char       		*read_line;
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
//			ft_show_argument_test(arg); // for test
			ft_set_echo();
			ft_system(arg);
			ft_free_argument(arg);
//			system("leaks minishell"); // for test
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

static void	ft_show_argument_test(t_argument *arg) // for test
{
	const char	*t_type_str_test[7] = {"ARGUMENT", "PIPE", "LT", "DLT", "GT", "DGT", "EOL"};
	char		**str;

	if (arg == 0)
		return ;
	printf("{ next tokenType: %s }\n", t_type_str_test[arg->next_token_type]);
	str = arg->pa_argument;
	while (*str != 0)
		printf("{ arg->pa_argument: %s }\n", *str++);
	if (arg->next != NULL)
	{
		printf(" |\n");
		printf(" v\n");
		ft_show_argument_test(arg->next);
	}
}
