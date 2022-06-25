/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 15:55:58 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/24 08:14:35 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_token	*p_syntax_error(enum e_token_type token_type);
static t_token	*p_end(t_token *cur_tok, t_argument *arg);
static t_token	*p_read(t_token *cur_tok, t_argument *arg, int idx);
static t_token	*p_init(t_token *cur_tok, t_argument *arg, int idx);

t_argument	*ft_parser(char *cmd_str, t_env_root *root_env)
{
	t_argument		*head_arg;
	t_argument		*cur_arg;
	t_token			*head_tok;
	t_token			*cur_tok;

	head_arg = NULL;
	head_tok = ft_tokenizer(cmd_str, root_env);
	if (head_tok == NULL)
		return (p_all_free(head_arg, head_tok));
	cur_tok = head_tok;
	while (cur_tok != NULL && cur_tok->token_type != EOL)
	{
		cur_arg = ft_init_argument(cur_tok, root_env);
		cur_tok = p_init(cur_tok, cur_arg, 0);
		ft_add_argument_back(&head_arg, cur_arg);
	}
	if (cur_tok == NULL)
		return (p_all_free(head_arg, head_tok));
	tk_free(head_tok);
	return (p_align_arg(head_arg));
}

static t_token	*p_init(t_token *cur_tok, t_argument *arg, int idx)
{
	if (cur_tok->token_type == ARGUMENT)
	{
		arg->pa_argument[idx] = ft_strdup(cur_tok->pa_str);
		return (p_read(cur_tok->next, arg, idx + 1));
	}
	else if (cur_tok->token_type == PIPE)
		return (p_syntax_error(cur_tok->token_type));
	return (p_read(cur_tok, arg, idx));
}

static t_token	*p_read(t_token *cur_tok, t_argument *arg, int idx)
{
	if (cur_tok->token_type == ARGUMENT)
	{
		arg->pa_argument[idx] = ft_strdup(cur_tok->pa_str);
		return (p_read(cur_tok->next, arg, idx + 1));
	}
	return (p_end(cur_tok, arg));
}

static t_token	*p_end(t_token *cur_tok, t_argument *arg)
{
	arg->next_token_type = cur_tok->token_type;
	if (cur_tok->token_type == EOL)
		return (cur_tok);
	if (cur_tok->token_type == PIPE)
	{
		if (cur_tok->next->token_type == EOL)
			return (p_extra_pipe(cur_tok, arg->env));
		return (cur_tok->next);
	}
	if (cur_tok->next->token_type == ARGUMENT)
	{
		if (cur_tok->token_type == DGT)
			return (p_heredoc(arg, cur_tok->next));
		return (cur_tok->next);
	}
	return (p_syntax_error(cur_tok->next->token_type));
}

static t_token	*p_syntax_error(enum e_token_type token_type)
{
	char	*err;

	if (token_type == LT)
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
	g_exit = 258;
	printf("minishell: syntax error near unexpected token `%s'\n", err);
	return (NULL);
}
