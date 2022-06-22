/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_add_pipe.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 04:37:37 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/22 20:58:15 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*ft_add_pipe(t_token *cur_token, t_env_root *env)
{
	char	*read_line;
	t_token	*add;

	read_line = readline("> ");
	if (read_line == NULL)
	{
		printf("minishell: syntax error: unexpected end of file\n");
		g_exit = 258;
		return (NULL);
	}
	add = ft_tokenizer(read_line, env);
	cur_token->next = add;
	return (cur_token->next);
}
