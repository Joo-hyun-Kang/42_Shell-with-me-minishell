/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 02:16:42 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/06 21:13:05 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_lexer	*ft_init_lexer(char *str)
{
	t_lexer	*l;

	l = (t_lexer *)malloc(sizeof(t_lexer));
	if (l == 0)
		return (0);
	ft_memset(l, 0, sizeof(t_lexer));
	l->pa_str = str;
	l->ch = *str;
	return (l);
}

void	ft_add_token(t_lexer *lexer, t_token_lst *token)
{
	t_token_lst	*iter;

	if (lexer->head_token == 0)
	{
		lexer->head_token = token;
		return ;
	}
	iter = lexer->head_token;
	while (iter->next)
		iter = iter->next;
	iter->next = token;
}

void	ft_analyze_string(t_lexer *lexer)
{
	t_token_lst		*token;

	token = ft_create_next_token(lexer);
	lexer->read_token = token;
	while (token->token_type != EOL)
	{
		ft_add_token(lexer, token);
		token = ft_create_next_token(lexer);
	}
}
