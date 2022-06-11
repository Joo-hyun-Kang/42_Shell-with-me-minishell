/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 15:55:58 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/12 05:22:57 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_get_token_type_char(enum e_token_type token_type)
{
	if (token_type == SEMICOLON)
		return (";");
	else if (token_type == LT)
		return (">");
	else if (token_type == DLT)
		return (">>");
	else if (token_type == GT)
		return ("<");
	else if (token_type == DGT)
		return ("<<");
	else
		return ("\\n");
}

t_token	*ft_read_token(t_token *cur_token, t_argument *out_arg, int index)
{
	if (cur_token->token_type == ARGUMENT)
	{
		out_arg->pa_argument[index] = cur_token->pa_str;
		return (ft_read_token(cur_token->next, out_arg, index + 1));
	}
	else
	{
		out_arg->next_token_type = cur_token->token_type;
		if (cur_token->token_type == EOL)
			return (cur_token);
		if (cur_token->next->token_type != ARGUMENT)
		{
			if (cur_token->token_type == PIPE)
				return (ft_add_additional_pipe(cur_token, *(out_arg->env)));
			else
			{
				printf("minishell: parse error near `%s'\n", \
						ft_get_token_type_char(cur_token->next->token_type));
				return (0);
			}
		}
		else if (cur_token->token_type == DGT)
			ft_heredoc(cur_token, *(out_arg->env));
		return (cur_token->next);
	}
}

t_token	*ft_read_token_init(t_token *cur_token, t_argument *arg, int idx)
{
	if (cur_token->token_type != ARGUMENT)
	{
		if (cur_token->token_type == PIPE)
		{
			printf("minishell: parse error near `|'\n");
			return (0);
		}
		else
			return (ft_read_token(cur_token, arg, -1));
	}
	else
	{
		arg->pa_argument[idx] = cur_token->pa_str;
		return (ft_read_token(cur_token->next, arg, idx + 1));
	}
}

t_argument	*ft_parser(char *input_command, char ***environment)
{
	t_argument		*head_arg;
	t_argument		*cur_arg;
	t_token			*head_token;
	t_token			*cur_token;

	if (*input_command == 0)
		return (0);
	head_arg = 0;
	head_token = ft_tokenizer(input_command, *environment);
	cur_token = head_token;
	while (cur_token->token_type != EOL)
	{
		cur_arg = ft_init_argument(cur_token, environment);
		cur_token = ft_read_token_init(cur_token, cur_arg, 0);
		if (cur_token == 0)
		{
			ft_add_argument_back(&head_arg, cur_arg);
			ft_free_argument(head_arg);
			ft_free_token(head_token);
			return (0);
		}
		ft_add_argument_back(&head_arg, cur_arg);
	}
	ft_free_token(head_token);
	return (head_arg);
}
