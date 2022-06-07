/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 20:05:14 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/07 15:45:10 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

const char	*t_type_str_test[8] = {"ARGUMENT", "PIPE", "SEMICOLON", "LT", "DLT", "GT", "DGT", "EOL"};

void	ft_print_token_test(t_lexer *lexer)
{
	t_token_lst	*iter;

	iter = lexer->head;
	while (iter != 0)
	{
		printf("{ tokenType: %-13s, tokenString: \"%s\" }\n", t_type_str_test[iter->token_type], iter->pa_str);
		iter = iter->next;
	}
}

int main(void)
{
	t_lexer	*lexer;

	lexer = (t_lexer *)malloc(sizeof(t_lexer));
	lexer->pa_str = "echo hello | wc -l | $ENV $ $cute ; echo Hello zzzzzzzzzz\"hello  what  is  your  name\"asdad\"asdas\"";
	printf("%s\n", lexer->pa_str);
	lexer->read_pos = 0;
	ft_tokenization(lexer);
	ft_print_token_test(lexer);
	
	t_lexer	*lexer2;
	lexer2 = (t_lexer *)malloc(sizeof(t_lexer));
	lexer2->pa_str = "echo front\"$ENVZZ what is your name\"rear";
	printf("%s\n", lexer2->pa_str);
	lexer2->read_pos = 0;
	ft_tokenization(lexer2);
	ft_print_token_test(lexer2);
	return 0;
}
