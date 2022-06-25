/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 16:05:45 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/23 02:01:29 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	lx_read(t_lexer *lexer)
{
	if (lexer->read_idx < lexer->cmd_str_size)
		++lexer->read_idx;
}

char	lx_chr(t_lexer *lexer)
{
	return (lexer->cmd_str[lexer->read_idx]);
}

char	lx_next_chr(t_lexer *lexer)
{
	return (lexer->cmd_str[lexer->read_idx + 1]);
}

char	*lx_ptr(t_lexer *lexer)
{
	return (&lexer->cmd_str[lexer->read_idx]);
}
