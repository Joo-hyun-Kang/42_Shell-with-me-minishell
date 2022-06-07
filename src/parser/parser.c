/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 15:55:58 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/06 21:09:06 by kanghyki         ###   ########.fr       */
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

char	**ft_init_pa_argument(int size)
{
	char	**pa_argument;

	pa_argument = (char **)malloc(sizeof(char *) * (size + 1));
	if (pa_argument == 0)
		return (0);
	ft_memset(pa_argument, 0, sizeof(char *) * (size + 1));
	pa_argument[size] = 0;
	return (pa_argument);
}

int	ft_get_pa_argument_size(t_lexer *lexer)
{
	int			i;
	t_token_lst	*iter;

	i = 0;
	iter = lexer->read_token;
	while (iter->token_type == ARGUMENT)
	{
		++i;
		iter = iter->next;
	}
	return (i);
}

void	ft_read_token(t_lexer *lexer)
{
	if (lexer->read_token->token_type != EOL)
		lexer->read_token = lexer->read_token->next;
}

t_token_lst	*ft_lookup_token(t_lexer *lexer)
{
	if (lexer->read_token->token_type != EOL)
		lexer->read_token = lexer->read_token->next;
	return (lexer->read_token);
}

t_argument	*ft_create_argument(t_lexer *lexer)
{
	int			index;
	int			pa_argument_size;
	t_argument	*argument;

	index = 0;
	argument = ft_init_argument();
	if (argument == 0)
		return (0);
	pa_argument_size = ft_get_pa_argument_size(lexer);
	argument->pa_argument = ft_init_pa_argument(pa_argument_size);
	if (argument->pa_argument == 0)
		return (0);
	while (lexer->read_token->token_type == ARGUMENT)
	{
		argument->pa_argument[index] = lexer->read_token->pa_str_or_null;
		ft_read_token(lexer);
		++index;
	}
	argument->next_token_type = lexer->read_token->token_type;
	ft_read_token(lexer);
	if (lexer->read_token->token_type != PIPE
		&& lexer->read_token->token_type != SEMICOLON
		&& lexer->read_token->token_type != EOL)
	{
		argument->use_only_redirection = lexer->read_token->pa_str_or_null;
		ft_read_token(lexer);
	}
	return (argument);
}

t_argument	*ft_add_argument(t_argument *head, t_argument *add)
{
	t_argument	*iter;

	iter = head;
	if (head == 0)
		return (add);
	while (iter->next)
		iter = iter->next;
	iter->next = add;
	return (head);
}

t_argument	*ft_parse_token(t_lexer *lexer)
{
	t_argument		*head;

	head = 0;
	while (lexer->read_token->token_type != EOL)
		head = ft_add_argument(head, ft_create_argument(lexer));
	return (head);
}

t_argument	*ft_parse_command(char *command)
{
	t_lexer		*lexer;
	t_argument	*argument;

	lexer = ft_init_lexer(command);
	ft_analyze_string(lexer);
	argument = ft_parse_token(lexer);
	return (argument);
}
