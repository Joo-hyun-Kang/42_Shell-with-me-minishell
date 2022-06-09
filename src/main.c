/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 22:42:17 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/09 14:05:42 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "excuse_cmd/cmd.h"

const char	*t_type_str_test[8] = {"ARGUMENT", "PIPE", "SEMICOLON", "LT", "DLT", "GT", "DGT", "EOL"};

char	**ft_copy_env(char **env)
{
	char	**duplicated;
	int		i;

	i = 0;
	while (env[i])
		++i;
	duplicated = (char **)malloc(sizeof(char *) + (i + 1));
	if (duplicated == 0)
		return (0);
	duplicated[i] = 0;
	i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		duplicated[i] = ft_strdup(env[i]);
		++i;
	}
	return (duplicated);
}

void	ft_show_argument_test(t_argument *arg)
{
	char	**str;

	if (arg == 0)
	{
		printf("{ t_argument: NULL }\n");
		return ;
	}
	printf("{ next tokenType: %s }\n", t_type_str_test[arg->next_token_type]);
	while (arg != 0)
	{
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
		str = readline("jokang@kanghyki$ ");
		add_history(str);
		arg = ft_str_to_argument(str);
		ft_show_argument_test(arg);
		if (arg)
			ft_system(arg);
		free(str);
		system("leaks minishell");
	}
	return (0);
}
