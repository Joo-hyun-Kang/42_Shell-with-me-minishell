/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_additional.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 04:37:37 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/15 11:44:19 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*ft_add_additional_pipe(t_token *cur_token, char **env)
{
	char	*read_line;
	t_token	*add;

	read_line = readline("> ");
	add = ft_tokenizer(read_line, env);
	free(read_line);
	cur_token->next = add;
	return (cur_token->next);
}

void	ft_heredoc(t_token *cur_token, char **env)
{
	char	*read_line;
	char	*new_str;
	char	*tmp;

	new_str = ft_strdup("");
	read_line = readline("> ");
	while (ft_strcmp(cur_token->next->pa_str, read_line) != 0)
	{
		new_str = ft_merge_str(new_str, read_line);
		tmp = new_str;
		new_str = ft_strjoin(new_str, "\n");
		free(tmp);
		read_line = readline("> ");
	}
	free(read_line);
	free(cur_token->next->pa_str);
	cur_token->next->pa_str = new_str;
}
