/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 22:42:17 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/08 20:45:57 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "excuse_cmd/cmd.h"

const char	*t_type_str_test[8] = {"ARGUMENT", "PIPE", "SEMICOLON", "LT", "DLT", "GT", "DGT", "EOL"};

int main(void)
{
	char        *str;
	t_argument	*arg;

	printf("%s", BANNER);
  while (1)
	{
		str = readline("jokang@kanghyki$ ");
		add_history(str);
    arg = ft_command_to_argument(str);
    while (arg != 0)
    {
      printf("=============\n");
      printf("{ next tokenType: %s }\n", t_type_str_test[arg->next_token_type]);
      while (*(arg->pa_argument) != 0)
      {
        printf("{ arg->pa_argument: %s }\n", *(arg->pa_argument));
        ++(arg->pa_argument);
      }
      arg = arg->next;
    }
    arg = ft_command_to_argument(str);
    //	t_lexer	*lexer2;
    ft_system(arg);
    free(str);
	}
	return (0);
}

