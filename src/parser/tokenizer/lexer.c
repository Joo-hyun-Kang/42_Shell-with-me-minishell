/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 23:03:18 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/21 17:52:12 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_lexer	*ft_init_lexer(char *cmd_str, t_env_root *env)
{
	t_lexer	*lexer;

	lexer = (t_lexer *)malloc(sizeof(t_lexer));
	if (lexer == NULL)
		return (NULL);
	ft_memset(lexer, 0, sizeof(t_lexer));
	lexer->cmd_str = cmd_str;
	lexer->cmd_str_size = ft_strlen(cmd_str);
	lexer->env = env;
	return (lexer);
}

void	ft_free_lexer(t_lexer *lexer)
{
	free(lexer->cmd_str);
	free(lexer);
}

void	ft_set_spos(t_lexer *lexer)
{
	lexer->spos = ft_cur_ptr(lexer);
}

char	*ft_get_spos(t_lexer *lexer)
{
	return (lexer->spos);
}

t_env_root	*ft_get_env(t_lexer *lexer)
{
	return (lexer->env);
}
