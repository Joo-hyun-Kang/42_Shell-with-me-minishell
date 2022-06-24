/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 23:03:18 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/24 10:00:36 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_lexer	*lx_init(char *cmd_str, t_env_root *env)
{
	t_lexer	*lexer;

	lexer = (t_lexer *)malloc(sizeof(t_lexer));
	if (lexer == NULL)
		ft_system_err(FAILED_MALLOC);
	ft_memset(lexer, 0, sizeof(t_lexer));
	lexer->cmd_str = cmd_str;
	lexer->cmd_str_size = ft_strlen(cmd_str);
	lexer->env = env;
	return (lexer);
}

void	lx_free(t_lexer *lexer)
{
	if (lexer->pa_str != NULL)
		free(lexer->pa_str);
	free(lexer->cmd_str);
	free(lexer);
}
