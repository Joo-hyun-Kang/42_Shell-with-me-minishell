/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 02:16:42 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/07 16:39:33 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_lexer	*ft_init_lexer(char *str)
{
	t_lexer	*lexer;

	lexer = (t_lexer *)malloc(sizeof(t_lexer));
	if (lexer == 0)
		return (0);
	ft_memset(lexer, 0, sizeof(t_lexer));
	lexer->pa_str = str;
	return (lexer);
}
