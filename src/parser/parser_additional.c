/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_additional.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 04:37:37 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/15 17:02:11 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*ft_add_pipe(t_token *cur_token, char **env)
{
	char	*read_line;
	t_token	*add;

	read_line = readline("> ");
	add = ft_tokenizer(read_line, env);
	free(read_line);
	cur_token->next = add;
	return (cur_token->next);
}

void	ft_replace_heredoc(t_token *cur_token, char **env)
{
	char	*read_line;
	char	*new_str;

	new_str = ft_strdup("");
	read_line = readline("> ");
	while (ft_strcmp(cur_token->pa_str, read_line) != 0)
	{
		new_str = ft_merge_str(new_str, read_line);
		new_str = ft_merge_str(new_str, ft_strdup("\n"));
		read_line = readline("> ");
	}
	free(read_line);
	free(cur_token->pa_str);
	cur_token->pa_str = new_str;
}
