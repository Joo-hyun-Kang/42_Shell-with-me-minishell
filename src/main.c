/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 22:42:17 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/12 05:26:24 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "excuse_cmd/cmd.h"

void	print_memory_test(char *dst, int size)
{
	int		i;

	i = 0;
	while (i < size)
	{
		if (i % 16 == 0)
			printf("%p: %2x", &dst[i], dst[i]);
		else if (i % 16 == 15)
		{
			printf("%02x ", dst[i]);
			for (int j = 15; j >= 0; --j)
				printf("%c", dst[i - j]);
			printf("\n");
		}
		else if (i % 2 == 1)
			printf("%02x ", dst[i]);
		else
			printf("%02x", dst[i]);
		++i;
	}
	if (i % 2 == 1)
		printf("   ");
	for (int j = i % 16; j > 0; --j)
	{
		printf("%c", dst[i - j]);
		if (j == 1)
			printf("\n");
	}
}

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
	char		**environment;
	t_argument	*arg;

	printf("%s", BANNER);
	environment = ft_copy_env(env);
	while (1)
	{
		str = readline("minishell-4.2$ ");
		add_history(str);
		arg = ft_parser(str, &environment);
		if (arg == 0)
		{
			free(str);
			continue ;
		}
		ft_show_argument_test(arg);
		ft_env_simple_command_test(arg);
//		if (arg)
//			ft_system(arg);
		free(str);
//		system("leaks minishell");
	}
	return (0);
}
