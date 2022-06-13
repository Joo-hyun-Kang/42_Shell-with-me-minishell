/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 22:42:17 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/13 16:59:43 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "excuse_cmd/cmd.h"

void	ft_print_memory(void *dst, int size)
{
	const char hex[16] = "0123456789abcdef";
	int		i;

	i = 0;
	while (i < size)
	{
		if (i % 16 == 0)
			printf("%p: ", &dst[i]);
		printf("%c", hex[((unsigned char)((char *)dst)[i]) / 16]);
		printf("%c", hex[((unsigned char)((char *)dst)[i]) % 16]);
		if (i % 2 == 1)
			printf(" ");
		if (i % 16 == 15)
		{
			for (int j = 15; j >= 0; --j)
				printf("%c", (unsigned char)((char *)dst)[i - j]);
			printf("\n");
		}
		++i;
	}
	if (i % 2 == 1)
		printf("   ");
	else
		printf("     ");
	for (int j = i % 16; j > 0; --j)
		printf("%c", (unsigned char)((char *)dst)[i - j]);
	printf("\n");
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

static void	sig_handler(int sig)
{
	static size_t	count = 0;

	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		printf("\n");
		rl_on_new_line();
		rl_redisplay();
	}
}

int main(int argc, char **argv, char **env)
{
	char        *str;
	char		**environment;
	t_argument	*arg;
	struct termios term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ICANON;    // non-canonical input 설정
	term.c_cc[VMIN] = 1;        // 최소 입력 버퍼 크기
	tcsetattr(STDIN_FILENO, TCSANOW, &term);

	printf("%s", BANNER);
	signal(SIGINT, sig_handler);
	environment = ft_envdup(env);
	while (1)
	{
		str = readline("minishell-4.2$ ");
		ft_print_memory(str, ft_strlen(str));
		add_history(str);
		arg = ft_parser(str, &environment);
		if (arg == 0)
		{
			free(str);
			continue ;
		}
		ft_show_argument_test(arg);
//		ft_env_simple_command_test(arg);
		ft_system(arg);
		free(str);
//		system("leaks minishell");
	}
	return (0);
}
