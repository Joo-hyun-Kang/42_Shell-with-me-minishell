/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 16:05:45 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/16 16:09:13 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	ft_read_lexer(t_lexer *lexer)
{
	char	c;

	c = ft_cur_char(lexer);
	if (lexer->read_idx < lexer->cmd_str_size)
		++lexer->read_idx;
	return (c);
}

char	ft_cur_char(t_lexer *lexer)
{
	return (lexer->cmd_str[lexer->read_idx]);
}

char	ft_next_char(t_lexer *lexer)
{
	return (lexer->cmd_str[lexer->read_idx + 1]);
}

char	*ft_cur_ptr(t_lexer *lexer)
{
	return (&lexer->cmd_str[lexer->read_idx]);
}

void	ft_replace_lexer_cmd_str(t_lexer *lexer, char *new_cmd_str)
{
	free(lexer->cmd_str);
	lexer->cmd_str = new_cmd_str;
	lexer->cmd_str_size = ft_strlen(new_cmd_str);
}
