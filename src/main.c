/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 22:42:17 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/22 20:02:12 by kanghyki         ###   ########.fr       */
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
	if (arg->next != NULL)
	{
		printf(" |\n");
		printf(" v\n");
		ft_show_argument_test(arg->next);
	}
}

void	ft_set_noecho(void)
{
	struct termios	term;

    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ECHOCTL);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	ft_set_echo(void)
{
	struct termios	term;

    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= ECHOCTL;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	ft_exit(void)
{
	printf("\033[1A\033[15Cexit\n");
	exit(g_exit);
}

int main(int argc, char **argv, char **env)
{
	char       		*str;
	t_argument		*arg;
	t_env_root		*root_env;

	g_exit = 0;
	ft_set_signal();
	ft_set_noecho();
	printf("%s", BANNER);
	root_env = ft_dpenv_to_bstenv(env);
	str = readline(READLINE);
	while (str != NULL)
	{
		add_history(str);
		arg = ft_parser(str, root_env);
		if (arg != NULL)
		{
//			ft_show_argument_test(arg);
			ft_set_echo();
			ft_system(arg);
			ft_free_argument(arg);
//		system("leaks minishell");
		}
		ft_set_noecho();
		ft_set_signal();
		str = readline(READLINE);
	}
	ft_exit();
	return (0);
}
