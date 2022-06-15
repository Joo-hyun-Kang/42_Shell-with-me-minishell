/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 15:55:58 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/15 22:34:32 by kanghyki         ###   ########.fr       */
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

t_token	*ft_no_eol(t_token *cur_token, t_argument *arg)
{
	if (cur_token->token_type == PIPE)
		return (ft_add_pipe(cur_token, arg->env));
	printf("minishell: parse error near `%s'\n", \
			ft_get_token_type_char(cur_token->next->token_type));
	return (NULL);
}

t_token	*ft_read_token(t_token *cur_token, t_argument *arg, int idx)
{
	if (cur_token->token_type == ARGUMENT)
	{
		arg->pa_argument[idx] = cur_token->pa_str;
		return (ft_read_token(cur_token->next, arg, idx + 1));
	}
	arg->next_token_type = cur_token->token_type;
	if (cur_token->token_type == EOL)
		return (cur_token);
	else if (cur_token->next->token_type != ARGUMENT)
		return (ft_no_eol(cur_token, arg));
	else if (cur_token->token_type == DGT)
		ft_replace_heredoc(cur_token->next, arg->env);
	return (cur_token->next);
}

t_token	*ft_read_token_init(t_token *cur_token, t_argument *arg, int idx)
{
	if (cur_token->token_type == ARGUMENT)
	{
		arg->pa_argument[idx] = cur_token->pa_str;
		return (ft_read_token(cur_token->next, arg, idx + 1));
	}
	else if (cur_token->token_type == PIPE)
	{
		printf("minishell: parse error near `|'\n");
		return (NULL);
	}
	return (ft_read_token(cur_token, arg, -1));
}

t_argument	*ft_parser(char *cmd_str, t_env_root *root_env)
{
	t_argument		*head_arg;
	t_argument		*cur_arg;
	t_token			*head_token;
	t_token			*cur_token;

	head_arg = NULL;
	head_token = ft_tokenizer(cmd_str, root_env);
	ft_add_token_back(&head_token, ft_init_token(NULL, EOL));
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
