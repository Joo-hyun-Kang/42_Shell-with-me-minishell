/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 22:42:17 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/10 09:19:42 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "excuse_cmd/cmd.h"

const char	*t_type_str_test[8] = {"ARGUMENT", "PIPE", "SEMICOLON", "LT", "DLT", "GT", "DGT", "EOL"};

void	ft_print_env_test(char **env)
{
	int i = 0;
	
	while (env[i])
	{
		printf("%s\n", env[i]);
		++i;
	}
}

void	ft_show_argument_test(t_argument *arg)
{
	char	**str;

	if (arg == 0)
	{
		printf("{ t_argument: NULL }\n");
		return ;
	}
	while (arg != 0)
	{
		printf("{ next tokenType: %s }\n", t_type_str_test[arg->next_token_type]);
		str = arg->pa_argument;
		while (*str != 0)
		{
			printf("{ arg->pa_argument: %s }\n", *str);
			++str;
		}
		arg = arg->next;
	}
}

int main(int argc, char **argv, char **env)
{
	char        *str;
	char		**environment;
	t_argument	*arg;

	printf("%s", BANNER);
	environment = ft_copy_env(env);
	while (1)
	{
		str = readline("minishell-4.2$ ");
		add_history(str);
//		arg = ft_str_to_argument(str);
//		environment = ft_set_env(environment, str, "ENV_FOR_TEST");
		environment = ft_unset_env(environment, str);
#if 1
		ft_print_env_test(environment);
#endif
//		ft_show_argument_test(arg);
//		if (arg)
//			ft_system(arg);
		free(str);
//		system("leaks minishell");
	}
	return (0);
}
