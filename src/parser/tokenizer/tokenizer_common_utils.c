/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_common_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 04:03:15 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/22 17:50:14 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_token	*ft_init_token(char *str, enum e_token_type t_type)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (new_token == NULL)
		return (NULL);
	ft_memset(new_token, 0, sizeof(t_token));
	new_token->pa_str = str;
	new_token->token_type = t_type;
	return (new_token);
}

void	ft_free_token(t_token *token)
{
	t_token	*prev;

	prev = token;
	while (token != NULL)
	{
		token = token->next;
		free(prev);
		prev = token;
	}
}

void	ft_add_token_back(t_token **head, t_token *new_token)
{
	t_token	*iter;

	if (*head == NULL)
		*head = new_token;
	else
	{
		iter = *head;
		while (iter->next != NULL)
			iter = iter->next;
		iter->next = new_token;
	}
}
