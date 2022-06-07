/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 15:55:58 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/07 19:10:13 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_argument	*ft_init_argument(void)
{
	t_argument	*argument;

	argument = (t_argument *)malloc(sizeof(t_argument));
	if (argument == 0)
		return (0);
	ft_memset(argument, 0, sizeof(t_argument));
	return (argument);
}

/*
 * ARGUMENT 없는 리다이렉션 ) > a.txt
 * 리다이렉션 후 ARGUMENT가 없음 ) echo hello >
 * ARGUMENT 없는 파이프
 */

int	ft_argument_size(t_token_lst *cur_token)
{
	int	i;

	i = 0;
	while (cur_token->next)
	{
		if (cur_token->token_type != ARGUMENT)
			break ;
		++i;
		cur_token = cur_token->next;
	}
	return (i);
}

void	ft_add_argument(t_argument **head, t_argument *argument)
{
	t_argument	*iter;

	if (*head == 0)
	{
		*head = argument;
		return ;
	}
	iter = *head;
	while (iter->next)
		iter = iter->next;
	iter->next = argument;
}

t_token_lst	*ft_read_token(t_token_lst *cur_token, t_argument *argument, int index)
{
	if (cur_token->token_type == EOL)
		return (cur_token);
	if (cur_token->token_type == ARGUMENT)
	{
		argument->pa_argument[index] = cur_token->pa_str;
		return (ft_read_token(cur_token->next, argument, index + 1));
	}
	else
	{
		argument->next_token_type = cur_token->token_type;
		return (cur_token->next);
	}
}

t_token_lst	*ft_read_token_only_type_argument(t_token_lst *cur_token, t_argument *argument, int index)
{
	if (cur_token->token_type == EOL)
		return (cur_token);
	if (cur_token->token_type != ARGUMENT)
	{
		printf("Error: parsing error\n");
		return (0);
	}
	argument->pa_argument[index] = cur_token->pa_str;
	return (ft_read_token(cur_token->next, argument, index + 1));
}

t_argument	*ft_command_to_argument(char *command)
{
	t_argument	*head;
	t_argument	*argument;
	t_lexer		*lexer;
	t_token_lst	*cur_token;

	lexer = ft_init_lexer(command);
	if (lexer == 0)
		return (0);
	ft_tokenization(lexer);
	head = 0;
	cur_token = lexer->head;
	while (cur_token->token_type != EOL)
	{
		// TODO: Malloc Execption
		argument = ft_init_argument();
		argument->pa_argument = (char **)malloc(sizeof(char *) * (ft_argument_size(cur_token) + 1));
		cur_token = ft_read_token_only_type_argument(cur_token, argument, 0);
		// parsing error
		if (cur_token == 0)
			return (0);
		ft_add_argument(&head, argument);
		argument = 0;
	}
	return (head);
}
