/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 04:03:15 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/23 04:49:09 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_token	*tk_init(t_lexer *lexer, enum e_token_type t_type)
{
	t_token	*new_tok;

	new_tok = (t_token *)malloc(sizeof(t_token));
	if (new_tok == NULL)
		return (NULL);
	ft_memset(new_tok, 0, sizeof(t_token));
	new_tok->token_type = t_type;
	if (lexer != NULL)
	{
		new_tok->pa_str = lexer->pa_str;
		lexer->pa_str = 0;
	}
	return (new_tok);
}

void	tk_free(t_token *token)
{
	t_token	*prev;

	prev = token;
	while (token != NULL)
	{
		if (token->pa_str != NULL)
			free(token->pa_str);
		token = token->next;
		free(prev);
		prev = token;
	}
}

void	tk_add_back(t_token **head, t_token *new_tok)
{
	t_token	*iter;

	if (*head == NULL)
		*head = new_tok;
	else
	{
		iter = *head;
		while (iter->next != NULL)
			iter = iter->next;
		iter->next = new_tok;
	}
}
