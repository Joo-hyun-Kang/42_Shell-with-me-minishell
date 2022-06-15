/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 23:03:18 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/15 23:28:18 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_lexer	*ft_init_lexer(char *cmd_str)
{
	t_lexer	*lexer;

	lexer = (t_lexer *)malloc(sizeof(t_lexer));
	if (lexer == NULL)
		return (NULL);
	ft_memset(lexer, 0, sizeof(t_lexer));
	lexer->cmd_str = cmd_str;
	lexer->cmd_str_size = ft_strlen(cmd_str);
	return (lexer);
}

void	ft_free_lexer(t_lexer *lexer)
{
	free(lexer->cmd_str);
	free(lexer);
}

void	ft_read_lexer(t_lexer *lexer)
{
	if (lexer->read_idx >= lexer->cmd_str_size)
		return ;
	else
		++lexer->read_idx;
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
