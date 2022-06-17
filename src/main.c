/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 22:42:17 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/18 04:30:48 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_show_argument_test(t_argument *arg)
{
	const char	*t_type_str_test[8] = {"ARGUMENT", "PIPE", "SEMICOLON", "LT", "DLT", "GT", "DGT", "EOL"};
	char		**str;

	if (arg == 0)
		return ;
	printf("{ next tokenType: %s }\n", t_type_str_test[arg->next_token_type]);
	str = arg->pa_argument;
	while (*str != 0)
		printf("{ arg->pa_argument: %s }\n", *str++);
	ft_show_argument_test(arg->next);
}

int main(int argc, char **argv, char **env)
{
	char        *str;
	t_argument	*arg;
	t_env_root	*root_env;

	g_exit = 0;
	printf("%s", BANNER);
	root_env = ft_dpenv_to_bstenv(env);
	while (1)
	{
		signal(SIGINT, sigint_handler);
		str = readline("minishell-4.2$ ");
		if (str == NULL)
			str = ft_strdup("exit");
		add_history(str);
		arg = ft_parser(str, root_env);
		if (arg == 0)
			continue ;
		ft_show_argument_test(arg);
		signal(SIGINT, sigint_handler_after_parsing);
		ft_system(arg);
		ft_free_argument(arg);
//		system("leaks minishell");
	}
	return (0);
}
