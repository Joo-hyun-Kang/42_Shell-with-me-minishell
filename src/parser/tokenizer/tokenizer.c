/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 20:30:32 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/23 01:59:21 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static t_token	*tk_create_arg(t_lexer *lexer);
static t_token	*tk_create_meta(t_lexer *lexer);

t_token	*ft_tokenizer(char *cmd_str, t_env_root *env)
{
	t_lexer	*lexer;
	t_token	*head;
	t_token	*new_token;

	head = 0;
	lexer = lx_init(cmd_str, env);
	while (lx_chr(lexer) != '\0')
	{
		lexer->pa_str = NULL;
		while (lx_chr(lexer) != '\0'
			&& ft_strchr(M_SPACE, lx_chr(lexer)) != NULL)
			lx_read(lexer);
		if (lx_chr(lexer) != '\0')
		{
			if (ft_strchr(M_META, lx_chr(lexer)) != NULL)
				new_token = tk_create_meta(lexer);
			else
				new_token = tk_create_arg(lexer);
			if (new_token != NULL)
				tk_add_back(&head, new_token);
		}
	}
	tk_add_back(&head, tk_init(NULL, EOL));
	lx_free(lexer);
	return (head);
}

static t_token	*tk_create_meta(t_lexer *lexer)
{
	enum e_token_type	token_type;

	token_type = 0;
	if (lx_chr(lexer) == '|')
		token_type = PIPE;
	else if (lx_chr(lexer) == '<')
	{
		token_type = GT;
		if (lx_next_chr(lexer) == '<')
			token_type = DGT;
	}
	else if (lx_chr(lexer) == '>')
	{
		token_type = LT;
		if (lx_next_chr(lexer) == '>')
			token_type = DLT;
	}
	if (token_type == DLT || token_type == DGT)
		lx_read(lexer);
	lx_read(lexer);
	return (tk_init(NULL, token_type));
}

static t_token	*tk_create_arg(t_lexer *lexer)
{
	while (lx_chr(lexer) != '\0')
	{
		lx_set_pos(lexer);
		while (lx_chr(lexer) != '\0' && ft_strchr(M_SEP, lx_chr(lexer)) == NULL)
			lx_read(lexer);
		lx_store_str(lexer);
		if (lx_chr(lexer) != '\0' && ft_strchr(M_QUOTE, lx_chr(lexer)) != NULL)
			tk_quote(lexer);
		else if (lx_chr(lexer) == M_HOME)
			tk_replace_home(lexer);
		else if (lx_chr(lexer) == M_ENV)
			tk_replace_env(lexer);
		if (lx_chr(lexer) != '\0'
				&& ft_strchr(M_SPACE, lx_chr(lexer)) != NULL
			|| ft_strchr(M_META, lx_chr(lexer)) != NULL)
			break ;
	}
	if (lexer->pa_str == NULL)
		return (NULL);
	return (tk_init(lexer->pa_str, ARGUMENT));
}
