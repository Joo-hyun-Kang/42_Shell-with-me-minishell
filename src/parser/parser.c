/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 15:55:58 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/21 11:15:41 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_syntax_error(enum e_token_type token_type)
{
	char	*err;

	if (token_type == SEMICOLON)
		err = ";";
	else if (token_type == LT)
		err = ">";
	else if (token_type == DLT)
		err = ">>";
	else if (token_type == GT)
		err = "<";
	else if (token_type == DGT)
		err = "<<";
	else if (token_type == PIPE)
		err = "|";
	else
		err = "newline";
	// #define EX_USAGE 258 syntax error in usage
	g_exit = 258;
	printf("minishell: syntax error near unexpected token `%s'\n", err);
}

t_token	*ft_read_token_end(t_token *cur_token, t_argument *arg)
{
	arg->next_token_type = cur_token->token_type;
	if (cur_token->token_type == EOL)
		return (cur_token);
	/* Pipe */
	if (cur_token->token_type == PIPE)
	{
		if (cur_token->next->token_type == EOL)
			return (ft_additional_pipe(cur_token, arg->env));
		else
			return (cur_token->next);
	}
	/* Redir */
	if (cur_token->next->token_type == ARGUMENT)
	{
		if (cur_token->token_type == DGT)
			ft_heredoc(arg, cur_token->next, arg->env);
		return (cur_token->next);
	}
	/* Error */
	ft_syntax_error(cur_token->next->token_type);
	return (NULL);
}

t_token	*ft_read_token_mid(t_token *cur_token, t_argument *arg, int idx)
{
	if (cur_token->token_type == ARGUMENT)
	{
		arg->pa_argument[idx] = cur_token->pa_str;
		return (ft_read_token_mid(cur_token->next, arg, idx + 1));
	}
	return (ft_read_token_end(cur_token, arg));
}

t_token	*ft_read_token_init(t_token *cur_token, t_argument *arg, int idx)
{
	if (cur_token->token_type == ARGUMENT)
	{
		arg->pa_argument[idx] = cur_token->pa_str;
		return (ft_read_token_mid(cur_token->next, arg, idx + 1));
	}
	else if (cur_token->token_type == PIPE)
	{
		ft_syntax_error(cur_token->token_type);
		return (NULL);
	}
	return (ft_read_token_mid(cur_token, arg, idx));
}

t_argument	*ft_parser(char *cmd_str, t_env_root *root_env)
{
	t_argument		*head_arg;
	t_argument		*cur_arg;
	t_token			*head_token;
	t_token			*cur_token;

	head_arg = NULL;
	head_token = ft_tokenizer(cmd_str, root_env);
	cur_token = head_token;
	while (cur_token->token_type != EOL)
	{
		cur_arg = ft_init_argument(cur_token, root_env);
		cur_token = ft_read_token_init(cur_token, cur_arg, 0);
		if (cur_token == NULL)
		{
			ft_add_argument_back(&head_arg, cur_arg);
			ft_free_argument(head_arg);
			ft_free_token(head_token);
			return (NULL);
		}
		ft_add_argument_back(&head_arg, cur_arg);
	}
	ft_free_token(head_token);
	return (head_arg);
}
