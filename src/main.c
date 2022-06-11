/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 22:42:17 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/12 03:15:44 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "excuse_cmd/cmd.h"

const char	*t_type_str_test[8] = {"ARGUMENT", "PIPE", "SEMICOLON", "LT", "DLT", "GT", "DGT", "EOL"};

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
//
//char	*ft_create_dict_test(char *key, char *value)
//{
//	const int	key_len = ft_strlen(key);
//	const int	value_len = ft_strlen(value);
//	char		*str;
//
//	str = (char *)malloc(sizeof(char) * (key_len + value_len + 2));
//	if (str == 0)
//		return (0);
//	ft_strlcpy(str, key, key_len + 1);
//	ft_strlcat(str, "=", key_len + 2);
//	ft_strlcat(str, value, key_len + value_len + 2);
//	return (str);
//}

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
