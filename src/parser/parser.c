/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 15:55:58 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/06 15:56:22 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_argument	*ft_parse_token(t_lexer *l)
{
	t_argument		*argument;
	t_token_lst		*iter;

	argument = (t_argument *)malloc(sizeof(t_argument));
	if (argument == 0)
		return (0);
	iter = l->head_token;
	while (iter->token_type != EOL)
	{
		while (iter->token_type == ARGUMENT)
		{
			// pa_argument에 담아줌
			iter = iter->next;
		}
		argument->next_token_type = iter->token_type;
		iter = iter->next;
	}
	return (argument);
}
